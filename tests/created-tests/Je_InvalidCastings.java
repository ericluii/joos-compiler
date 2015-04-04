public class Je_InvalidCastings {
    public Je_InvalidCastings() {
        /* all of these are invalid castings
         * by JLS 5.2 assignment rules. T can't be assigned
         * to S nor S can be assigned to T.
         * 
         * Please check this for instanceof too */
        Object o = (Je_InvalidCastings[]) new Je_InvalidCastings();
        Object o2 = (String[]) new Je_InvalidCastings();
        Object o3 = (Cloneable[]) new Integer[2];
    }
}
