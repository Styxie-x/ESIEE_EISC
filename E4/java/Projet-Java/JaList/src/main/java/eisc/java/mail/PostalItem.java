package eisc.java.mail;

public interface PostalItem {
    Address sender();
    Address addressee();
    double mass();
}