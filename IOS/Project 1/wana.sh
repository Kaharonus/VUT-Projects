#!/bin/sh

POSIXLY_CORRECT=yes

filterType=""
logFiles=""
cmd=""
files=""

while [ $# -gt 0 ]
do
    key="$1"
    case "$key" in
        "-"*)
            shift
            filterType="$filterType
$key"
            filterType="$filterType
$1"
            if [ "$key" = "-a" -o "$key" = "-b" ]; then
                dateNew=$(echo $1 | grep -Eo "^[0-9]{4}-[0-1][0-9]-[0-3][0-9][[:space:]][0-2][0-9]:[0-5][0-9]:[0-5][0-9]$")
                if [ "$dateNew" = "" ]; then
                    echo "$key date is not in valid format"
                    return 1
                fi
            fi
            ;;
        *-h*|*-i*|*-u*|*-l*)
            cmd=$key
            ;;
        *)
            files="$files $key"
            ;;
    esac
    shift
done

#Check data
 for file in $files; do
    if [ ! -f $file ]; then
        echo "File $file does not exist."
        return 1
    fi
 done


lines=""

#Read lines

if [ "$files" != "" ]; then
    for file in $files; do
         case "$file" in
            *".gz")
                lines="$lines\n$(zcat $file)"
                ;;
            *)
                lines="$lines\n$(cat $file)"
                ;;
        esac
    done
else
    echo Press enter with empty line to exit:
    while [ 0 ]; do
        read line
        if [ "$line" = "" ]; then
            break
        fi
        lines="$lines\n$line"
    done
fi


IFS='
'

value=""
filterAfter(){
    filteredLines=""
    for line in $lines
    do
        logdate=$(echo "$line"|grep -Eo \\[\.*\\]| cut -c 2- | rev | cut -c 8- | rev | sed 's/\//\-/g' | sed '/:/s// /' )
        epochTime=""
        if [ $(uname) = "FreeBSD" ]; then
            logdate=$(date  -jf '%d-%b-%Y %H:%M:%S' "$logdate" +%s)
            epochTime=$(date  -jf "%Y-%m-%d %H:%M:%S" "$value" +%s)
        else
            epochTime=$(date -d $value +%s)
            logdate=$(date -d $logdate +%s)
        fi
            if [ $logdate -gt $epochTime ]
            then
                filteredLines="$filteredLines
$line"
            fi
    done
    lines=$filteredLines
}

filterBefore(){
    filteredLines=""
    for line in $lines
    do
        logdate=$(echo "$line"|grep -Eo \\[\.*\\]| cut -c 2- | rev | cut -c 8- | rev | sed 's/\//\-/g' | sed '/:/s// /' )
        epochTime=""
        if [ $(uname) = "FreeBSD" ]; then
            logdate=$(date  -jf '%d-%b-%Y %H:%M:%S' "$logdate" +%s)
            epochTime=$(date  -jf "%Y-%m-%d %H:%M:%S" "$value" +%s)
        else
            epochTime=$(date -d $value +%s)
            logdate=$(date -d $logdate +%s)
        fi
            if [ $logdate -lt $epochTime ]
            then
                filteredLines="$filteredLines
$line"
            fi
    done
    lines=$filteredLines
}

filterIP(){
    filteredLines=""
    for line in $lines
    do
       ip=$(echo "$line"|grep -Eo '^\S*')
       if [ $ip = $value ]; then
        filteredLines="$filteredLines
$line"
       fi
    done
    lines=$filteredLines
}

filterUrl(){
    value=$(printf $value | sed "s@\\\\@@g")
    filteredLines=""
    for line in $lines
    do
        filteredUrl=$(echo "$line"|grep -Eo "[[:space:]]/.*[[:space:]]H" | cut -c 2- | rev | cut -c 3- | rev)
        if [ "$filteredUrl" = "$value" ]; then
            filteredLines="$filteredLines
$line"
       fi
    done
    lines=$filteredLines
}

origLang=$LANG
LANG=en.us

for filter in $filterType; do
    case "$filter" in
         #save last filter value as type of filter
        "-"*)
        value="$filter"
        ;;
        #use previous filter type to determine what will happen now
        *)

            case "$value" in
            "-a")
                value="$filter"
                filterAfter
                ;;
            "-b")
                value="$filter"
                filterBefore
                ;;
            "-ip")
                value="$filter"
                filterIP
                ;;
            "-uri")
                value="$filter"
                filterUrl
                ;;

            esac
            ;;
    esac

done


LANG=$origLang
result=""

getHosts(){
    tmp=""
    for line in $lines
    do
        ip=$( echo "$line"|grep -Eo '^\S*')
        tmp="$tmp\n$ip"
    done
    tmp=$(printf $tmp |sort| uniq)
    for line in $tmp; do
        host=$(host $line)
        isFound=$(echo $host | grep -Eo '^Host')
        if [ "$isFound" = "" ]; then
            host=$(echo $host | grep -Eo "pointer[[:space:]].*$" | cut -c 9-)
            result="$result\n$host"
        fi
    done
    result="$(echo $result | cut -c 1- )"
}


getIP() {
    for line in $lines
    do
        ip=$(echo "$line"|grep -Eo '^\S*')
        result="$result\n$ip"
    done
    result=$(printf $result |sort|uniq)
}

listURI() {
    for line in $lines
    do
        ip=$(echo "$line"|grep -Eo "[[:space:]]/.*[[:space:]]H" | cut -c 2- | rev | cut -c 3- | rev)
        result="$result\n$ip"
    done
    result=$(echo $result |sort| uniq)
}


getHistIP(){
    tmp=""
    for line in $lines
    do
        ip=$(echo "$line"|grep -Eo '^\S*')
        result="$result\n$ip"
    done
    result=$(printf $result | sed '1d;'|sort|uniq -c | cut -c 7- | sort -r)
    for line in $result
    do
        count=$(echo "$line"|grep -Eo '^.*[[:space:]]' | rev | cut -c 2- | rev)
        ip=$(echo "$line"|grep -Eo '[[:space:]].*'  | cut -c 2-)
        tmp="$tmp$ip ($count): "
        for i in `seq 1 $count`
        do
            tmp="$tmp#"
        done
        tmp="$tmp\n"
    done
    result=$(printf $tmp)
}

getHistLoad(){
    tmp=""
    for line in $lines
    do
        time="$(echo "$line"|grep -Eo \\[\.*\\] | cut -c 2-15):00"
        result="$result\n$time"
    done
    result=$(printf $result| sed '1d;'|sort|uniq -c | cut -c 7- )
    for line in $result
    do
        count=$(echo "$line"|grep -Eo '^.*[[:space:]]' | rev | cut -c 2- | rev)
        ip=$(echo "$line"|grep -Eo '[[:space:]].*'  | cut -c 2- | sed 's/\//\-/g' | sed '0,/:/s// /')
        tmp="$tmp$ip ($count): "
        for i in `seq 1 $count`
        do
            tmp="$tmp#"
        done
        tmp="$tmp\n"
    done
result=$(printf $tmp)

}

case $cmd in
    "list-ip")
        getIP
        ;;
    "list-hosts")
        getHosts
        ;;
    "list-uri")
        listURI
        ;;
    "hist-load")
        getHistLoad
        ;;
    "hist-ip")
        getHistIP
        ;;
    "")
    result=$lines
     ;;
esac

#Write results

for line in $result ; do
    line=$(echo $line | sed 's/%3A/:/g')
    line=$(echo $line | sed 's/%2F/\//g')
    printf "$line"
    printf "\n"
done
return 0