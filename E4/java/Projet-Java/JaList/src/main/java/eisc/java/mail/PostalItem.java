package eisc.java.mail;

public interface PostalItem {
    Address sender();
    Address addressee();
    double mass();

    default int compareTo(PostalItem postalItem){
        if(this.mass() < postalItem.mass()) return -1;
        if(this.mass() > postalItem.mass()) return 1;
        return 0;
    }

    default boolean isInternational(){
        return this.sender().country().equals(this.addressee().country());
    }
}