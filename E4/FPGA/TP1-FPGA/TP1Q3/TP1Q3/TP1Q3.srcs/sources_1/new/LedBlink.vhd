library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- 1. Fréquence de l'horloge FPGA : f_clk = 100 MHz (période T_clk = 10 ns).
-- 2. Relation : f_led = f_clk / 100_000_000 (basculement toutes les 0,5 s = 50_000_000 cycles).
-- 3. Si la LED était reliée directement à clk (100 MHz), elle clignoterait trop vite
--    pour l'œil humain (persistance rétinienne) et paraîtrait allumée en continu à 50 % de luminosité.

entity LedBlink is
    Generic (
        MAX_COUNT : integer := 50_000_000
    );
    Port ( 
        clk : in  STD_LOGIC;
        led : out STD_LOGIC
    );
end LedBlink;

architecture Behavioral of LedBlink is
    signal counter : integer range 0 to MAX_COUNT - 1 := 0;
    signal led_reg : STD_LOGIC := '0';
begin

    process(clk)
    begin
        if rising_edge(clk) then
            if counter = MAX_COUNT - 1 then
                counter <= 0;
                led_reg <= not led_reg;
            else
                counter <= counter + 1;
            end if;
        end if;
    end process;

    led <= led_reg;

end Behavioral;