package com.example;

public class HelloWorld {
    public static void main(String[] arg) {
        Person Me = new Person("styxie",2021);
        String name = null;
        String birthyear = null;

        if (arg.length >= 1) {
            name = arg[0];
        }
        if (arg.length >= 2) {
            birthyear = arg[1];
        }

        if (name == null) {
            System.out.println("No name supplied");
        } else {
            System.out.println(name);
        }

        if (birthyear == null) {
            System.out.println("No birth year supplied");
        } else {
            System.out.println(birthyear);
        }

        boolean isValid = false;
        do {
            String result = IO.readln("what is your birthyear? ");
            try {
                int year = Integer.parseInt(result);
                if (year >= 2027 || year <= 0) {
                    System.out.println("error : wrong format or date");
                } else {
                    System.out.println("Valid year: " + year);
                    isValid = true; 
                }
            } catch (NumberFormatException e) {
                System.out.println("error : not a valid number");
            }
        } while (!isValid);
    }
}
