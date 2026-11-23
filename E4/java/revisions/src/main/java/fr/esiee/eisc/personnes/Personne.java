package fr.esiee.eisc.personnes;

public class Personne implements Boorn, Named{

    private String name;
    private final int birthYear;

    public String getName() {
        return name;
    }
    public void setName(String newName) {
        if(newName.length() >= 2){
            this.name = name;
        } else {
            throw new IllegalArgumentException("name too short");
        }

    }
    public int getBirthYear() {
        return birthYear;
    }




    public Personne(String name, int birthYear) {
        this.name = name;
        this.birthYear = birthYear;
    }
    public Personne(String name) {
        this(name, birthYear: 2000);
    }
}
