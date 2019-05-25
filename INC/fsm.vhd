-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------

-- 1408710833

architecture behavioral of fsm is
   type t_state is (CODE_BEGIN, CODE3, CODE35, CODE352, CODE3521, CODE35217, CODE352177, CODE3521777, CODE35217770, CODE352177708, CODE3521777084 ,CODE1, CODE14, CODE140 ,CODE1408, CODE14087, CODE140871, CODE1408710, CODE14087108, CODE140871083, CODE1408710833, WAIT_FOR_CONFIRM, PRINT_ERROR, PRINT_SUCCESS, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= CODE_BEGIN;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE_BEGIN =>
      next_state <= CODE_BEGIN;
		if (KEY(1) = '1') then
         next_state <= CODE1;
      elsif (KEY(3) = '1') then
         next_state <= CODE3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
		  next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE3 =>
      next_state <= CODE3;
		if (KEY(5) = '1') then
			next_state <= CODE35;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
		  next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE35 =>
      next_state <= CODE35;
      if (KEY(2) = '1') then
         next_state <= CODE352;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
      next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE352 =>
      next_state <= CODE352;
      if (KEY(1) = '1') then
         next_state <= CODE3521;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE3521 =>
      next_state <= CODE3521;
      if (KEY(7) = '1') then
         next_state <= CODE35217;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE35217 =>
      next_state <= CODE35217;
      if (KEY(7) = '1') then
         next_state <= CODE352177;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE352177 =>
      next_state <= CODE352177;
      if (KEY(7) = '1') then
         next_state <= CODE3521777;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE3521777 =>
      next_state <= CODE3521777;
      if (KEY(0) = '1') then
         next_state <= CODE35217770;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE35217770 =>
      next_state <= CODE35217770;
      if (KEY(8) = '1') then
         next_state <= CODE352177708;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE352177708 =>
   next_state <= CODE352177708;
   if (KEY(4) = '1') then
      next_state <= CODE3521777084;
   elsif (KEY(15) = '1') then
      next_state <= PRINT_ERROR; 
   elsif (KEY(14 downto 0) /= "000000000000000") then
      next_state <= WAIT_FOR_CONFIRM;
   end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE3521777084 =>
      next_state <= CODE3521777084;
      if (KEY(15) = '1') then
         next_state <= PRINT_SUCCESS; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when CODE1 =>
      next_state <= CODE1;
		if (KEY(4) = '1') then
			next_state <= CODE14;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
		  next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when CODE14 =>
      next_state <= CODE14;
		if (KEY(0) = '1') then
			next_state <= CODE140;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
		  next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when CODE140 =>
      next_state <= CODE140;
		if (KEY(8) = '1') then
			next_state <= CODE1408;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
		  next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when CODE1408 =>
      next_state <= CODE1408;
      if (KEY(7) = '1') then
         next_state <= CODE14087;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE14087 =>
      next_state <= CODE14087;
      if (KEY(1) = '1') then
         next_state <= CODE140871;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE140871 =>
      next_state <= CODE140871;
      if (KEY(0) = '1') then
         next_state <= CODE1408710;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE1408710 =>
      next_state <= CODE1408710;
      if (KEY(8) = '1') then
         next_state <= CODE14087108;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE14087108 =>
      next_state <= CODE14087108;
      if (KEY(3) = '1') then
         next_state <= CODE140871083;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE140871083 =>
      next_state <= CODE140871083;
      if (KEY(3) = '1') then
         next_state <= CODE1408710833;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CODE1408710833 =>
      next_state <= CODE1408710833;
      if (KEY(15) = '1') then
         next_state <= PRINT_SUCCESS; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WAIT_FOR_CONFIRM;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -


	when WAIT_FOR_CONFIRM =>
      next_state <= WAIT_FOR_CONFIRM;
      if (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ERROR =>
      next_state <= PRINT_ERROR;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SUCCESS =>
      next_state <= PRINT_SUCCESS;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= CODE_BEGIN; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= CODE_BEGIN;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ERROR =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
	 when PRINT_SUCCESS =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
		 if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;

