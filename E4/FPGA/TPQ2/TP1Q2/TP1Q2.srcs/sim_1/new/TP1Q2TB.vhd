library IEEE;
use IEEE.Std_logic_1164.all;

entity tp1Q2_tb is
end;

architecture bench of tp1Q2_tb is

  -- Déclaration du composant à tester
  component tp1Q2
      Port ( 
          D : in STD_LOGIC;
          Q : out STD_LOGIC;
          clk : in STD_LOGIC
      );
  end component;

  signal D: STD_LOGIC := '0';
  signal Q: STD_LOGIC;
  signal clk: STD_LOGIC := '0';

  constant clk_period : time := 10 ns;

begin

  -- Instanciation et connexion des signaux
  uut: tp1Q2 port map ( D   => D,
                        Q   => Q,
                        clk => clk );

  -- Génération de l'horloge
  clk_process: process
  begin
      clk <= '0';
      wait for clk_period/2;
      clk <= '1';
      wait for clk_period/2;
  end process;

  -- Génération des stimuli (Test)
  stimulus: process
  begin
      wait for 12 ns;
      
      D <= '1';
      wait for 20 ns;
      
      D <= '0';
      wait for 10 ns;
      
      D <= '1';
      wait for 3 ns;
      D <= '0';

      wait;
  end process;

end bench;