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
           O <= "00000001" when "0000" ,--0
                "01001111" when "0001" ,--1
                "00010010" when "0010" ,--2
                "00000110" when "0011" ,--3
                "01001100" when "0100" ,--4
                "00100100" when "0101" ,--5
                "00100000" when "0110" ,--6
                "00001111" when "0111" ,--7
                "00000000" when "1000" ,--8
                "00000100" when "1001" ,--9
                "00001000" when "1010" ,--A
                "00000000" when "1011" ,--B "1100000" for lower case b 
                "00110001" when "1100" ,--C
                "00000001" when "1101" ,--D "1000010" for lower case d
                "00110000" when "1110" ,--E
                "00111000" when "1111" ,--F
                "01111111" when others; -- All OFF
end Behavioral;