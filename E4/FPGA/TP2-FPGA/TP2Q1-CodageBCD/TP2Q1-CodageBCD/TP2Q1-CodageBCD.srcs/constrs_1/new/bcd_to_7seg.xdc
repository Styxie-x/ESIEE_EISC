## ============================================================================
## Contraintes Nexys A7-100T - TP2 Question 1 (Décodeur BCD vers 7 segments)
## ============================================================================

## Interrupteurs SW0 à SW3 -> Entrée B(3 downto 0)
set_property -dict { PACKAGE_PIN J15   IOSTANDARD LVCMOS33 } [get_ports { B[0] }]; # SW0 (LSB)
set_property -dict { PACKAGE_PIN L16   IOSTANDARD LVCMOS33 } [get_ports { B[1] }]; # SW1
set_property -dict { PACKAGE_PIN M13   IOSTANDARD LVCMOS33 } [get_ports { B[2] }]; # SW2
set_property -dict { PACKAGE_PIN R15   IOSTANDARD LVCMOS33 } [get_ports { B[3] }]; # SW3 (MSB)

## Cathodes 7 segments -> Sortie O(7 downto 0) (Active-low)
## Ordre dans ton code : O(7)=DP, O(6)=CA, O(5)=CB, O(4)=CC, O(3)=CD, O(2)=CE, O(1)=CF, O(0)=CG
set_property -dict { PACKAGE_PIN H15   IOSTANDARD LVCMOS33 } [get_ports { O[7] }]; # DP (Point décimal)
set_property -dict { PACKAGE_PIN T10   IOSTANDARD LVCMOS33 } [get_ports { O[6] }]; # CA[cite: 7]
set_property -dict { PACKAGE_PIN R10   IOSTANDARD LVCMOS33 } [get_ports { O[5] }]; # CB[cite: 7]
set_property -dict { PACKAGE_PIN K16   IOSTANDARD LVCMOS33 } [get_ports { O[4] }]; # CC[cite: 7]
set_property -dict { PACKAGE_PIN K13   IOSTANDARD LVCMOS33 } [get_ports { O[3] }]; # CD[cite: 7]
set_property -dict { PACKAGE_PIN P15   IOSTANDARD LVCMOS33 } [get_ports { O[2] }]; # CE[cite: 7]
set_property -dict { PACKAGE_PIN T11   IOSTANDARD LVCMOS33 } [get_ports { O[1] }]; # CF[cite: 7]
set_property -dict { PACKAGE_PIN L18   IOSTANDARD LVCMOS33 } [get_ports { O[0] }]; # CG[cite: 7]

## Anodes des 8 afficheurs (Active-low) - Décommenter si tu ajoutes AN(7 downto 0) à ton entity
# set_property -dict { PACKAGE_PIN J17   IOSTANDARD LVCMOS33 } [get_ports { AN[0] }]; # AN0 (Afficheur de droite - actif à '0')
# set_property -dict { PACKAGE_PIN J18   IOSTANDARD LVCMOS33 } [get_ports { AN[1] }]; # AN1
# set_property -dict { PACKAGE_PIN T9    IOSTANDARD LVCMOS33 } [get_ports { AN[2] }]; # AN2
# set_property -dict { PACKAGE_PIN J14   IOSTANDARD LVCMOS33 } [get_ports { AN[3] }]; # AN3
# set_property -dict { PACKAGE_PIN P14   IOSTANDARD LVCMOS33 } [get_ports { AN[4] }]; # AN4
# set_property -dict { PACKAGE_PIN T14   IOSTANDARD LVCMOS33 } [get_ports { AN[5] }]; # AN5
# set_property -dict { PACKAGE_PIN K2    IOSTANDARD LVCMOS33 } [get_ports { AN[6] }]; # AN6
# set_property -dict { PACKAGE_PIN U13   IOSTANDARD LVCMOS33 } [get_ports { AN[7] }]; # AN7