-- Testbench created online at:
--   https://www.doulos.com/knowhow/perl/vhdl-testbench-creation-using-perl/
-- Copyright Doulos Ltd

library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;

entity LedBlink_tb is
end;

architecture bench of LedBlink_tb is

  component LedBlink
      Generic (
          MAX_COUNT : integer := 50_000_000
      );
      Port ( 
          clk : in  STD_LOGIC;
          led : out STD_LOGIC
      );
  end component;

  signal clk: STD_LOGIC;
  signal led: STD_LOGIC;

  constant clock_period: time := 10 ns;
  signal stop_the_clock: boolean := false;

begin

  -- Surcharge de MAX_COUNT à 5 pour observer plusieurs basculements rapidement
  uut: LedBlink generic map ( MAX_COUNT => 5 )
                port map    ( clk       => clk,
                              led       => led );

  stimulus: process
  begin
    -- On laisse tourner pendant 300 ns (soit 30 cycles d'horloge = 6 basculements de la LED)
    wait for 300 ns;

    stop_the_clock <= true;
    wait;
  end process;

  clocking: process
  begin
    while not stop_the_clock loop
      clk <= '0', '1' after clock_period / 2;
      wait for clock_period;
    end loop;
    wait;
  end process;

end;