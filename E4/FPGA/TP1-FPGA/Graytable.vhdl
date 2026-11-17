library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity bin_gray is
    Port (
        b2, b1, b0 : in  std_logic;
        g2, g1, g0 : out std_logic
    );
end entity bin_gray;

architecture behavioral of bin_gray is
process(b2, b1, b0)
begin
    case std_logic_vector'(b2 & b1 & b0) is
        when "000" => g2 <= '0'; g1 <= '0'; g0 <= '0';
        when "001" => g2 <= '0'; g1 <= '0'; g0 <= '1';
        when "010" => g2 <= '0'; g1 <= '0'; g0 <= '1';
        when "011" => g2 <= '0'; g1 <= '1'; g0 <= '0';
        when "100" => g2 <= '1'; g1 <= '1'; g0 <= '0';
        when "101" => g2 <= '1'; g1 <= '1'; g0 <= '1';
        when "110" => g2 <= '1'; g1 <= '0'; g0 <= '1';
        when "111" => g2 <= '1'; g1 <= '0'; g0 <= '0';
        when others => g2 <= '0'; g1 <= '0'; g0 <= '0';
    end case;
end process;
end architecture behavioral;
