package HelW;

import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        var arraylen = 10;
        long[] fact = new long[arraylen];

        for (int i = 0; i < arraylen; i++) {
            fact[i] = Integer.parseInt(IO.readln("Donner un nombre : ").trim());
        }

        // 1. Tri obligatoire pour la médiane
        Arrays.sort(fact);

        // 2. Calcul de la médiane
        double mediane;
        if (arraylen % 2 == 1) {
            mediane = fact[arraylen / 2];
        } else {
            // Moyenne des deux valeurs centrales (division par 2.0 pour garder les décimales)
            mediane = (fact[(arraylen / 2) - 1] + fact[arraylen / 2]) / 2.0;
        }

        System.out.println("Tableau trié : " + Arrays.toString(fact));
        System.out.println("Médiane : " + mediane);
    }
}