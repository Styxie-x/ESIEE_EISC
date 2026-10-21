package eisc.java.mail;

public record Parcel(Address sender, Address addressee, double mass, double volume) implements PostalItem {
}