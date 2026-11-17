----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/09/2026 12:23:51 PM
-- Design Name: 
-- Module Name: tp1Q2 - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------

-- Table de vérité d'une bascule D (sur front montant)
-- =========================================================================
-- | Horloge (CLK)       | Entrée (D) | Sortie (Q) | Description           |
-- |---------------------|------------|------------|-----------------------|
-- | 0, 1 ou desc. (↓)   |     X      |   Q_prev   | Mémorisation          |
-- | Front montant (↑)   |     0      |     0      | Mise à 0              |
-- | Front montant (↑)   |     1      |     1      | Mise à 1              |
-- =========================================================================

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity tp1Q2 is
    Port ( D : in STD_LOGIC;
           Q : out STD_LOGIC;
           clk : in STD_LOGIC);
end tp1Q2;

architecture Behavioral of tp1Q2 is
begin

    process(clk)
    begin
        -- Détection du front montant de l'horloge
        if rising_edge(clk) then
            Q <= D;
        end if;
    end process;

end Behavioral;