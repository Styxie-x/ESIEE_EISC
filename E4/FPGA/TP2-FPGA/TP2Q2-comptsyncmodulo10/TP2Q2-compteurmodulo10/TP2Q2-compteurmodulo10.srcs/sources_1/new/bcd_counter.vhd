library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bcd0to9_counter is
    port(
        clk    : in  std_logic;
        reset  : in  std_logic;
        enable : in  std_logic;
        carry  : out std_logic;
        bcd4   : out std_logic_vector(3 downto 0)
    );
end bcd0to9_counter;

architecture Behavioral of bcd0to9_counter is
    signal counter : integer range 0 to 9 := 0;
begin
    process(clk, reset)
    begin
        if reset = '1' then
            counter <= 0;
        elsif rising_edge(clk) and enable = '1' then
                if counter < 9 then
                    counter <= counter + 1;
                else
                    counter <= 0;
                end if;
        end if;
    end process;
    bcd4 <= std_logic_vector(to_unsigned(counter, 4));
    carry <= '1' when (counter = 9 and enable = '1') else '0';
end Behavioral;