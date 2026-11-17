library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity bcd_to_7seg is
--  Port ( );
    port(
    B : in std_logic_vector(3 downto 0);
    O : out std_logic_vector(7 downto 0)
    );
end bcd_to_7seg;

architecture Behavioral of bcd_to_7seg is
begin
with B select -- Point , A, B, C, D, E, F, G, 
           O <= "10000001" when "0000" ,--0
                "11001111" when "0001" ,--1
                "10010010" when "0010" ,--2
                "10000110" when "0011" ,--3
                "11001100" when "0100" ,--4
                "10100100" when "0101" ,--5
                "10100000" when "0110" ,--6
                "10001111" when "0111" ,--7
                "10000000" when "1000" ,--8
                "10000100" when "1001" ,--9

                "01111111" when others; -- All OFF
end Behavioral;