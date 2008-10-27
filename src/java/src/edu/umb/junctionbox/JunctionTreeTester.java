package edu.umb.junctionbox;
import java.util.HashMap;

/**A class to run a bunch of sanity checks on a JunctionTree after evidence
 * propagation etc.*/
public class JunctionTreeTester{
    protected HashMap<String,Variable> variables;
    protected HashMap<String,Observation> obs;
    protected HashMap<String,Table> tables;
    protected SWIGTYPE_p_BNet bn;
    public JunctionTreeTester( HashMap<String,Variable> variables,
                               HashMap<String,Observation> obs,
                               HashMap<String,Table> tables,
                               SWIGTYPE_p_BNet bn
                               ){
        this.variables = variables;
        this.obs = obs;
        this.tables = tables;
        this.bn = bn;
    }
    public void testMarginalConsistency(){
        for(Variable v:variables.values()){
            System.out.println("Now checking variable "+ v.name);
            SWIGTYPE_p_List containingPots = junctionbox.BNet_AllContClqPot(this.bn, v.pointer);
            System.out.print("\t There are(is) ");
            System.out.print("" + junctionbox.List_Length(containingPots));
            System.out.println(" clique(s) containing this variable.");
        }

    }
}
