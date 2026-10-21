package com.example;

public class Person {
    private String name;
    private int birthyear;
    public Person(String name, int  birthyear) {
        this.name = name;
        this.birthyear = birthyear;
    }
    public String getNameString(){
        return name;
    }
    public int getBirthYear(){
        return birthyear;
    }
}
