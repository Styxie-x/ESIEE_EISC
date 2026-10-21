package eisc.java.mail;

public record Mail(Address sender, Address addressee, double mass) implements PostalItem {
}