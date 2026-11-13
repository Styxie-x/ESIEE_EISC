-- Testbench created online at:
--   https://www.doulos.com/knowhow/perl/vhdl-testbench-creation-using-perl/
-- Copyright Doulos Ltd

library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;

entity tp1_tb is
end;

architecture bench of tp1_tb is

  component tp1
      Port ( 
          a : in  STD_LOGIC;
          b : in  STD_LOGIC;
          s : out STD_LOGIC
      );
  end component;

  signal a: STD_LOGIC;
  signal b: STD_LOGIC;
  signal s: STD_LOGIC ;

begin

  uut: tp1 port map ( a => a,
                      b => b,
                      s => s );

stimulus: process
  begin

    a <= '0';
    b <= '0';
    wait for 100ns;

    a <= '0';
    b <= '1';
    wait for 100ns;

    a <= '1';
    b <= '0';
    wait for 100ns;

    a <= '1';
    b <= '1';
    wait;

  end process;

end;