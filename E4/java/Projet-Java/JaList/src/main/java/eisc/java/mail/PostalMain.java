package eisc.java.mail;

import java.util.ArrayList;
import java.util.List;

public class PostalMain {
    public static void main(String[] args) {
        List<PostalItem> items = new ArrayList<>();

        for (int i = 1; i <= 50; i++) {
            Address sender = new Address(
                    "Expéditeur " + i,
                    i + " Rue de Paris",
                    "Paris",
                    "France"
            );

            Address addressee = new Address(
                    "Destinataire " + i,
                    (i * 2) + " Avenue de Lyon",
                    "Lyon",
                    "France"
            );

            double mass = Math.round((0.1 * i + 0.2) * 100.0) / 100.0;

            if (i % 2 == 1) {
                // Création d'un courrier (Mail)
                items.add(new Mail(sender, addressee, mass));
            } else {
                // Création d'un colis (Parcel) avec un volume
                double volume = Math.round((0.05 * i) * 100.0) / 100.0;
                items.add(new Parcel(sender, addressee, mass, volume));
            }
        }

        // Affichage des 50 éléments
        System.out.println("=== Liste des 50 envois postaux ===");
        for (int i = 0; i < items.size(); i++) {
            PostalItem item = items.get(i);
            System.out.printf("[%02d] %s%n", i + 1, item);
        }
    }
}

