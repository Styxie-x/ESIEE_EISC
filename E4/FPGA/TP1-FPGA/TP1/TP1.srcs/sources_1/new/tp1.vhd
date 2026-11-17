----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/09/2026 11:11:56 AM
-- Design Name: 
-- Module Name: tp1 - Behavioral
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

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tp1 is
    Port ( 
        a : in  STD_LOGIC;
        b : in  STD_LOGIC;
        s : out STD_LOGIC
    );
end tp1;

architecture Behavioral of tp1 is
begin

    s <= a and b;

end Behavioral;