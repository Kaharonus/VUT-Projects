#!/usr/bin/env python3

import types

class Polynomial:

    # ctor definition that supports any number of arguments 
    def __init__(self, *args ,**kwargs):
        self.data = {}
        params=[]
        #This means that args is used instead of kwargs
        if(len(args) != 0):
            if(type(args[0]) == int):
                params = args
            else:
                #select first arg (the list)
                params = args[0]
        #Use kwargs    
        elif len(kwargs) != 0:
            tmp=[]
            #get all keys
            for var in kwargs:
                #remove x
                tmp.append(var[1:])
            tmp=int(max(tmp))+1
            i=0
            while  i < tmp:
                #add all keys including those we dont use.
                params.append(kwargs["x"+str(i)] if ("x"+str(i)) in kwargs else 0)
                i+=1         
        #empty ctor that I sometimes use in other methods
        else:
            return 
        for idx,arg in enumerate(params):
            #data is stored as a dictionary in format nx^m where n=value and m=key
            self.data[idx] = arg

    #overload for **
    def __pow__(self, num):
        orig = self.data.values()
        result = self.data.values()
        i = 1
        #honestly just lightly edited https://stackoverflow.com/questions/5413158/multiplying-polynomials-in-python
        #works by multiplying the expression with itself requested number of times
        while i < num:
            new = result
            res = [0]*(len(new)+len(orig)-1)
            for o1,i1 in enumerate(new):
                for o2,i2 in enumerate(orig):
                    res[o1+o2] += i1*i2
            result = res
            i+=1 
        #since result is a list I can just create new instance using it     
        return Polynomial(result)

    
    def at_value(self, num, otherNum=0):
        result = 0
        otherResult = 0
        for key in self.data:
            result += pow(num,key) * self.data[key]
            if(otherNum != 0):
                otherResult += pow(otherNum,key) * self.data[key]
        #if otherResult was calculated use other - result
        return otherResult - result if otherResult != 0 else result

    def derivative(self):
        newData = {}
        for key in self.data:
            #when key is 0 we are trying to derivate a constant which is nothing
            if(key != 0):
                newData[key-1] = self.data[key]*key
        #create new instatnce and copy the data
        tmp = Polynomial()
        tmp.data = newData.copy()
        return tmp           

    #overload for ==
    def __eq__(self, other):
        #easies way to do this is to compare the string results
        return str(self) == str(other)

    #overload for +
    def __add__(self, other):
        #copy data to a new object
        tmp = Polynomial()
        tmp.data = self.data.copy()
        for key in other.data:
            #if the key is present add to id, else add the key
            if key in tmp.data:
                tmp.data[key] += other.data[key]
            else:
                tmp.data[key] = other.data[key]
        return tmp
      
    #overload for conversion to string
    def __str__(self):
        result = ""
        #total number of results
        i = len(self.data) - 1 
        while i > 0:
            #when data[i] would be 0 in formula mx^n would m=0 (which would make that part = 0) so we can skip it
            if(self.data[i] != 0):
                #Dont add + or - at the begining
                if(result != ""):
                    result += " + " if self.data[i] > 0 else " - "
                # mx^n number = m, when data == 1 we dont add this number so we dont get results like 1x^2
                number = str(abs(self.data[i])) if abs(self.data[i]) > 1 else ""
                #mx^n power = n, when i == 1 we dont add this number so we dont get results like 3x^1
                power = (("^"+str(i)) if i > 1 else "")
                result += (number + "x" + power)    
            i -= 1
        #add the constant at the end
        if(i != - 1 and self.data[0] != 0):
             result += " + " if self.data[0] > 0 else " - "
             result += str(abs(self.data[0]))
        #When results was not changed the result is automatically 0
        if(result == ""):
            result = "0"
        return result       

def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()
