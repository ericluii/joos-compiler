package jlalr;

import java.util.*;
import java.io.*;

/*
   Copyright 2006,2008,2009 Ondrej Lhotak. All rights reserved.

   Permission is granted for study use by
   students registered in CS 444, Winter 2014
   term.

   The contents of this file may not be
   published, in whole or in part, in print
   or electronic form.

   The contents of this file may be included
   in work submitted for CS 444 assignments in
   Winter 2014.  The contents of this file may
   not be submitted, in whole or in part, for
   credit in any other course.

*/

/*
 * JLALR constructs LALR(1) and SLR(1) parse tables from a grammar, using
 * the algorithms described in chapter 3 of Appel, _Modern Compiler       
 * Implementation in Java, second edition_, 2002. JLALR reads a grammar
 * on standard input, and writes the generated grammar and parse tables on
 * standard output.                                                            
 * 
*/

/** Represents an item (a dotted production). */
class Item {
    Production rule; // the production
    int pos; // position of the dot (0 <= pos <= rule.rhs.size())
    String lookahead; // the lookahead terminal
    private Item( Production rule, int pos, String lookahead ) {
        this.rule = rule;
        this.pos = pos;
        this.lookahead = lookahead;
    }
    private static Map<Pair<Production, Pair<Integer, String>>, Item> map =
        new HashMap<Pair<Production, Pair<Integer, String>>, Item>();
    public static Item v(Production rule, int pos, String lookahead) {
        Pair<Production, Pair<Integer, String>> triple = 
            new Pair<Production, Pair<Integer, String>>(
                    rule, new Pair<Integer, String>(pos, lookahead));
        Item ret = map.get(triple);
        if(ret == null) {
            ret = new Item(rule, pos, lookahead);
            map.put(triple, ret);
        }
        return ret;
    }
    public static Item v(Production rule, int pos) {
        return v(rule, pos, "");
    }
    /** Returns true if the dot is not at the end of the RHS. */
    public boolean hasNextSym() {
        return pos < rule.rhs.length;
    }
    /** Returns the symbol immediately after the dot. */
    public String nextSym() {
        if(!hasNextSym()) throw new RuntimeException("Internal error: getting next symbol of an item with no next symbol");
        return rule.rhs[pos];
    }
    /** Returns the item obtained by advancing (shifting) the dot by one
     *  symbol. */
    public Item advance() {
        if(!hasNextSym()) throw new RuntimeException("Internal error: advancing an item with no next symbol");
        return Item.v(rule, pos+1, lookahead);
    }
    public String toString() {
        StringBuffer ret = new StringBuffer();
        ret.append("(");
        ret.append(rule.lhs);
        ret.append(" ->");
        int i;
        for(i = 0; i < pos; i++) ret.append(" "+rule.rhs[i]);
        ret.append(" ##");
        for(; i < rule.rhs.length; i++) ret.append(" "+rule.rhs[i]);
        ret.append(", ");
        ret.append(lookahead);
        ret.append(")");
        return ret.toString();
    }
}
class State {
    public Set<Item> items;
    private State(Set<Item> items) {
        this.items = items;
    }
    private static Map<Set<Item>, State> map = new HashMap<Set<Item>, State>();
    public static State v(Set<Item> items) {
        State ret = map.get(items);
        if(ret == null) {
            ret = new State(items);
            map.put(items, ret);
        }
        return ret;
    }
    public String toString() {
        StringBuffer ret = new StringBuffer();
        ret.append("\n");
        for(Item item : items) {
            ret.append(item);
            ret.append("\n");
        }
        return ret.toString();
    }
}
/** Represents a parser action (shift or reduce). */
abstract class Action {
}
/** Represents a shift parser action. */
class ShiftAction extends Action {
    State nextState; // the automaton state to move to after the shift
    public ShiftAction(State nextState) {
        this.nextState = nextState;
    }
    public int hashCode() { return nextState.hashCode(); }
    public boolean equals(Object other) {
        if(!(other instanceof ShiftAction)) return false;
        ShiftAction o = (ShiftAction) other;
        return nextState.equals(o.nextState);
    }
    public String toString() {
        return "shift " + nextState;
    }
}
/** Represents a reduce parser action. */
class ReduceAction extends Action {
    Production rule; // the production to reduce by
    public ReduceAction(Production rule) {
        this.rule = rule;
    }
    public int hashCode() { return rule.hashCode(); }
    public boolean equals(Object other) {
        if(!(other instanceof ReduceAction)) return false;
        ReduceAction o = (ReduceAction) other;
        return rule.equals(o.rule);
    }
    public String toString() {
        return "reduce " + rule;
    }
}
/** Utility class representing a pair of arbitrary objects. */
class Pair<A,B> {
    public Pair( A o1, B o2 ) { this.o1 = o1; this.o2 = o2; }
    public int hashCode() {
        return o1.hashCode() + o2.hashCode();
    }
    public boolean equals( Object other ) {
        if( other instanceof Pair ) {
            Pair p = (Pair) other;
            return o1.equals( p.o1 ) && o2.equals( p.o2 );
        } else return false;
    }
    public String toString() {
        return "Pair "+o1+","+o2;
    }
    public A getO1() { return o1; }
    public B getO2() { return o2; }

    protected A o1;
    protected B o2;
}


/** The main LALR/SLR generator class. */
class Generator {
    /** The context-free grammar. */
    Grammar grammar;
    /** A map from each non-terminal to the productions that expand it. */
    Map<String,List<Production>> lhsToRules = new HashMap<String,List<Production>>();

    public Generator(Grammar grammar) {
        this.grammar = grammar;
        for(String nonterm : grammar.nonterminals) {
            lhsToRules.put(nonterm, new ArrayList<Production>());
        }
        for(Production p : grammar.productions) {
            List<Production> mapRules = lhsToRules.get(p.lhs);
            mapRules.add(p);
        }
    }

    /** Compute the closure of a set of items using the algorithm of
     * Appel, p. 60 */
    public Set<Item> closure(Set<Item> i) {
        boolean change;
        while(true) {
            Set<Item> oldI = new HashSet<Item>(i);
            for( Item item : oldI ) {
                if(!item.hasNextSym()) continue;
                String x = item.nextSym();
                if(grammar.isTerminal(x)) continue;
                for( Production r : lhsToRules.get(x)) {
                    i.add(Item.v(r, 0));
                }
            }
            if( i.equals(oldI) ) return i;
        }
    }
    /** Compute the goto set for state i and symbol x, using the algorithm
     * of Appel, p. 60 */
    public Set<Item> goto_(Set<Item> i, String x) {
        Set<Item> j = new HashSet<Item>();
        for( Item item : i ) {
            if( !item.hasNextSym() ) continue;
            if( !item.nextSym().equals(x) ) continue;
            j.add(item.advance());
        }
        return closure(j);
    }
    private Map<List<String>, Set<String>> generalFirstCache = new HashMap<List<String>, Set<String>>();
    /** Compute the generalized first using the definition of Appel, p. 50. */
    public Set<String> generalFirst(List<String> l) {
        Set<String> ret = generalFirstCache.get(l);
        if(ret == null) {
            ret = new HashSet<String>();
            if(l.isEmpty()) {
                return ret;
            } else {
                ret.addAll(first.get(l.get(0)));
                int i = 0;
                while(i < l.size()-1 && nullable.contains(l.get(i))) {
                    ret.addAll(first.get(l.get(i+1)));
                    i++;
                }
            }
            generalFirstCache.put(l, ret);
        }
        return ret;
    }

    private Map<Set<Item>, Set<Item>> closureCache = new HashMap<Set<Item>, Set<Item>>();
    /** Compute the closure of a set of LR(1) items using the algorithm of
     * Appel, p. 63 */
    public Set<Item> lr1_closure(Set<Item> i) {
        boolean change;
        Set<Item> ret = closureCache.get(i);
        if(ret == null) {
            Set<Item> origI = new HashSet<Item>(i);
            Queue<Item> q = new LinkedList<Item>(i);
            while(!q.isEmpty()) {
                Item item = q.remove();
                if(!item.hasNextSym()) continue;
                String x = item.nextSym();
                if(grammar.isTerminal(x)) continue;
                List<String> betaz = new ArrayList<String>();
                for(int p = item.pos+1; p < item.rule.rhs.length; p++) {
                    betaz.add(item.rule.rhs[p]);
                }
                betaz.add(item.lookahead);
                Collection<String> ws = generalFirst(betaz);
                for( Production r : lhsToRules.get(x)) {
                    for( String w : ws ) {
                        Item newItem = Item.v(r, 0, w);
                        if(i.add(newItem)) q.add(newItem);
                    }
                }
            }
            closureCache.put(origI, i);
            ret = i;
        }
        return ret;
    }


    private Map<Pair<State, String>, State> gotoCache = new HashMap<Pair<State, String>, State>();
    /** Compute the LR(1) goto set for state i and symbol x, using the algorithm
     * of Appel, p. 63 */
    public State lr1_goto_(State i, String x) {
        Pair<State, String> pair = new Pair<State, String>(i, x);
        State ret = gotoCache.get(pair);
        if(ret == null) {
            Set<Item> j = new HashSet<Item>();
            for( Item item : i.items ) {
                if( !item.hasNextSym() ) continue;
                if( !item.nextSym().equals(x) ) continue;
                j.add(item.advance());
            }
            ret = State.v(lr1_closure(j));
            gotoCache.put(pair, ret);
        }
        return ret;
    }
    /** Add the action a to the parse table for the state state and
     * symbol sym. Report a conflict if the table already contains
     * an action for the same state and symbol. */
    private boolean addAction( State state, String sym, Action a ) {
        boolean ret = false;
        Pair<State,String> p = new Pair<State,String>(state, sym);
        Action old = table.get(p);
        if(old != null && !old.equals(a)) {
            throw new Error(
                "Conflict on symbol "+sym+" in state "+state+"\n"+
                "Possible actions:\n"+
                old+"\n"+a);
        }
        if(old == null || !old.equals(a)) ret = true;
        table.put(p, a);
        return ret;
    }
    /** Return true if all the symbols in l are in the set nullable. */
    private boolean allNullable(String[] l) {
        return allNullable(l, 0, l.length);
    }
    /** Return true if the symbols start..end in l are in the set nullable. */
    private boolean allNullable(String[] l, int start, int end) {
        boolean ret = true;
        for(int i = start; i < end; i++) {
            if(!nullable.contains(l[i])) ret = false;
        }
        return ret;
    }
    // The NULLABLE, FIRST, and FOLLOW sets. See Appel, pp. 47-49
    Set<String> nullable = new HashSet<String>();
    Map<String,Set<String>> first = new HashMap<String,Set<String>>();
    Map<String,Set<String>> follow = new HashMap<String,Set<String>>();
    /** Computes NULLABLE, FIRST, and FOLLOW sets using the algorithm
     * of Appel, p. 49 */
    public void computeFirstFollowNullable() {
        for( String z : grammar.syms() ) {
            first.put(z, new HashSet<String>());
            if(grammar.isTerminal(z)) first.get(z).add(z);
            follow.put(z, new HashSet<String>());
        }
        boolean change;
        do {
            change = false;
            for( Production rule : grammar.productions ) {
                if(allNullable(rule.rhs)) {
                    if( nullable.add(rule.lhs) ) change = true;
                }
                int k = rule.rhs.length;
                for(int i = 0; i < k; i++) {
                    if(allNullable(rule.rhs, 0, i)) {
                        if( first.get(rule.lhs).addAll(
                                first.get(rule.rhs[i])))
                            change = true;
                    }
                    if(allNullable(rule.rhs, i+1,k)) {
                        if( follow.get(rule.rhs[i]).addAll(
                                follow.get(rule.lhs)))
                            change = true;
                    }
                    for(int j = i+1; j < k; j++) {
                        if(allNullable(rule.rhs, i+1,j)) {
                            if( follow.get(rule.rhs[i]).addAll(
                                    first.get(rule.rhs[j])))
                                change = true;
                        }
                    }
                }
            }
        } while(change);
    }
    /** The computed parse table. */
    Map<Pair<State,String>,Action> table = 
        new HashMap<Pair<State,String>,Action>();
    State initialState;

    /** Generates the LR(0) parse table using the algorithms on 
     * pp. 60 of Appel. */
    public void generateLR0Table() {
        Set<Item> startRuleSet = new HashSet<Item>();
        for(Production r : lhsToRules.get(grammar.start)) {
            startRuleSet.add(Item.v(r, 0));
        }
        initialState = State.v(closure(startRuleSet));
        Set<State> t = new HashSet<State>();
        t.add(initialState);
        boolean change;
        // compute goto actions
        do {
            change = false;
            for( State i : new ArrayList<State>(t) ) {
                for( Item item : i.items ) {
                    if(!item.hasNextSym()) continue;
                    String x = item.nextSym();
                    State j = State.v(goto_(i.items, x));
                    if(t.add(j)) change = true;
                    if(addAction(i, x, new ShiftAction(j))) change = true;
                }
            }
        } while(change);
        // compute reduce actions
        for( State i : t ) {
            for( Item item : i.items ) {
                if( item.hasNextSym() ) continue;
                for( String x : grammar.syms() ) {
                    addAction(i, x, new ReduceAction(item.rule));
                }
            }
        }
    }

    /** Generates the SLR(1) parse table using the algorithms on 
     * pp. 60 and 62 of Appel. */
    public void generateSLR1Table() {
        Set<Item> startRuleSet = new HashSet<Item>();
        for(Production r : lhsToRules.get(grammar.start)) {
            startRuleSet.add(Item.v(r, 0));
        }
        initialState = State.v(closure(startRuleSet));
        Set<State> t = new HashSet<State>();
        t.add(initialState);
        boolean change;
        // compute goto actions
        do {
            change = false;
            for( State i : new ArrayList<State>(t) ) {
                for( Item item : i.items ) {
                    if(!item.hasNextSym()) continue;
                    String x = item.nextSym();
                    State j = State.v(goto_(i.items, x));
                    if(t.add(j)) change = true;
                    if(addAction(i, x, new ShiftAction(j))) change = true;
                }
            }
        } while(change);
        // compute reduce actions
        for( State i : t ) {
            for( Item item : i.items ) {
                if( item.hasNextSym() ) continue;
                for( String x : follow.get(item.rule.lhs) ) {
                    addAction(i, x, new ReduceAction(item.rule));
                }
            }
        }
    }

    /** Generates the LR(1) parse table using the algorithms on 
     * pp. 60, 62, and 64 of Appel. */
    public void generateLR1Table() {
        Set<Item> startRuleSet = new HashSet<Item>();
        System.err.println("Computing start state");
        for(Production r : lhsToRules.get(grammar.start)) {
            startRuleSet.add(Item.v(r, 0, grammar.terminals.iterator().next()));
        }
        initialState = State.v(lr1_closure(startRuleSet));
        Set<State> t = new HashSet<State>();
        t.add(initialState);
        Queue<State> q = new LinkedList<State>();
        q.add(initialState);
        // compute goto actions
        System.err.println("Computing goto actions");
        while(!q.isEmpty()) {
            State i = q.remove();
            for( Item item : i.items ) {
                if(!item.hasNextSym()) continue;
                String x = item.nextSym();
                State j = lr1_goto_(i, x);
                if(t.add(j)) {
                    System.err.print(".");
                    q.add(j);
                }
                addAction(i, x, new ShiftAction(j));
            }
        }
        // compute reduce actions
        System.err.println("Computing reduce actions");
        for( State i : t ) {
            for( Item item : i.items ) {
                if( item.hasNextSym() ) continue;
                addAction(i, item.lookahead, new ReduceAction(item.rule));
            }
        }
    }
    public Set<Item> core(Set<Item> items) {
        Set<Item> ret = new HashSet<Item>();
        for(Item item : items) {
            ret.add(Item.v(item.rule, item.pos));
        }
        return ret;
    }
    /** Generates the LALR(1) parse table using the algorithms on 
     * pp. 60, 62, and 64 of Appel. */
    public void generateLALR1Table() {
        Set<Item> startRuleSet = new HashSet<Item>();
        System.err.println("Computing start state");
        for(Production r : lhsToRules.get(grammar.start)) {
            startRuleSet.add(Item.v(r, 0, grammar.terminals.iterator().next()));
        }
        initialState = State.v(lr1_closure(startRuleSet));
        Set<State> t = new HashSet<State>();
        t.add(initialState);
        Queue<State> q = new LinkedList<State>();
        q.add(initialState);
        System.err.println("Computing states");
        while(!q.isEmpty()) {
            State i = q.remove();
            for( Item item : i.items ) {
                if(!item.hasNextSym()) continue;
                String x = item.nextSym();
                State j = lr1_goto_(i, x);
                if(t.add(j)) {
                    System.err.print(".");
                    q.add(j);
                }
            }
        }
        System.err.println("Merging states");
        Map<Set<Item>, Set<Item>> coreToState = new HashMap<Set<Item>, Set<Item>>();
        for(State state : t) {
            Set<Item> items = state.items;
            Set<Item> core = core(items);
            Set<Item> accum = coreToState.get(core);
            if(accum == null) {
                System.err.print(".");
                accum = new HashSet<Item>(items);
                coreToState.put(core, accum);
            } else accum.addAll(items);
        }
        Map<Set<Item>, State> coreToStateState = new HashMap<Set<Item>, State>();
        for(State state : t) {
            Set<Item> core = core(state.items);
            coreToStateState.put(core, State.v(coreToState.get(core)));
        }
        // compute goto actions
        System.err.println("Computing goto actions");
        t.clear();
        initialState = State.v(coreToState.get(core(initialState.items)));
        t.add(initialState);
        q.add(initialState);
        while(!q.isEmpty()) {
            State i = q.remove();
            for( Item item : i.items ) {
                if(!item.hasNextSym()) continue;
                String x = item.nextSym();
                State j = lr1_goto_(i, x);
                j = coreToStateState.get(core(j.items));
                if(t.add(j)) {
                    System.err.print(".");
                    q.add(j);
                }
                addAction(i, x, new ShiftAction(j));
            }
        }
        // compute reduce actions
        System.err.println("Computing reduce actions");
        for( State i : t ) {
            for( Item item : i.items ) {
                if( item.hasNextSym() ) continue;
                addAction(i, item.lookahead, new ReduceAction(item.rule));
            }
        }
    }
    /** Print the elements of a list separated by spaces. */
    public static String listToString(List l) {
        StringBuffer ret = new StringBuffer();
        boolean first = true;
        for( Object o : l ) {
            if( !first ) ret.append(" ");
            first = false;
            ret.append(o);
        }
        return ret.toString();
    }
    /** Produce output according to the output specification. */
    public void generateOutput() {
        Map<Production,Integer> ruleMap = new HashMap<Production,Integer>();
        int i = 0;
        for(Production r : grammar.productions) {
            ruleMap.put(r, i++);
        }
        Map<State,Integer> stateMap = new HashMap<State,Integer>();
        i = 0;
        stateMap.put(initialState, i++);
        for(Action a : table.values()) {
            if(!(a instanceof ShiftAction)) continue;
            State state = ((ShiftAction)a).nextState;
            if(!stateMap.containsKey(state)) {
                stateMap.put(state, i++);
            }
        }
        for(Pair<State,String> key : table.keySet()) {
            State state = key.getO1();
            if(!stateMap.containsKey(state)) {
                stateMap.put(state, i++);
            }
        }
        System.out.println(i);
        System.out.println(table.size());
        for(Map.Entry<Pair<State,String>,Action> e : table.entrySet()) {
            Pair<State,String> p = e.getKey();
            System.out.print(stateMap.get(p.getO1())+" "+p.getO2()+" ");
            Action a = e.getValue();
            if(a instanceof ShiftAction) {
                System.out.println("shift "+
                        stateMap.get(((ShiftAction)a).nextState));
            } else if(a instanceof ReduceAction) {
                System.out.println("reduce "+
                        ruleMap.get(((ReduceAction)a).rule));
            } else throw new Error("Internal error: unknown action");
        }
    }
}

class Jlr1 {
    public static final void main(String[] args) {
        Grammar grammar;
        try {
            grammar = Util.readGrammar(new Scanner(System.in));
            Util.writeGrammar(grammar);
        } catch(Error e) {
            System.err.println("Error reading grammar: "+e);
            System.exit(1);
            return;
        }
        Generator jlr = new Generator(grammar);
        try {
            jlr.computeFirstFollowNullable();
            jlr.generateLR1Table();
            jlr.generateOutput();
        } catch(Error e) {
            System.err.println("Error performing LR(1) construction: "+e);
            System.exit(1);
            return;
        } 
    }
}
class Jlr0 {
    public static final void main(String[] args) {
        Grammar grammar;
        try {
            grammar = Util.readGrammar(new Scanner(System.in));
            Util.writeGrammar(grammar);
        } catch(Error e) {
            System.err.println("Error reading grammar: "+e);
            System.exit(1);
            return;
        }
        Generator jlr = new Generator(grammar);
        try {
            jlr.computeFirstFollowNullable();
            jlr.generateLR0Table();
            jlr.generateOutput();
        } catch(Error e) {
            System.err.println("Error performing LR(0) construction: "+e);
            System.exit(1);
            return;
        } 
    }
}
class Jslr1 {
    public static final void main(String[] args) {
        Grammar grammar;
        try {
            grammar = Util.readGrammar(new Scanner(System.in));
            Util.writeGrammar(grammar);
        } catch(Error e) {
            System.err.println("Error reading grammar: "+e);
            System.exit(1);
            return;
        }
        Generator jslr = new Generator(grammar);
        try {
            jslr.computeFirstFollowNullable();
            jslr.generateSLR1Table();
            jslr.generateOutput();
        } catch(Error e) {
            System.err.println("Error performing SLR(1) construction: "+e);
            System.exit(1);
            return;
        } 
    }
}
public class Jlalr1 {
    public static final void main(String[] args) {
        Grammar grammar;
        try {
            grammar = Util.readGrammar(new Scanner(System.in));
            Util.writeGrammar(grammar);
        } catch(Error e) {
            System.err.println("Error reading grammar: "+e);
            System.exit(1);
            return;
        }
        Generator jlalr = new Generator(grammar);
        try {
            jlalr.computeFirstFollowNullable();
            jlalr.generateLALR1Table();
            jlalr.generateOutput();
        } catch(Error e) {
            System.err.println("Error performing LALR(1) construction: "+e);
            System.exit(1);
            return;
        } 
    }
}
/** A production in the grammar. */
class Production {
    public String lhs;
    public String[] rhs;
    private Production(String lhs, String[] rhs) {
        this.lhs = lhs; this.rhs = rhs;
    }
    private static Map<Pair<String, String[]>, Production> map =
        new HashMap<Pair<String, String[]>, Production>();
    public static Production v(String lhs, String[] rhs) {
        Pair<String, String[]> pair = new Pair<String, String[]>(lhs, rhs);
        Production ret = map.get(pair);
        if(ret == null) {
            ret = new Production(lhs, rhs);
            map.put(pair, ret);
        }
        return ret;
    }
    public String toString() {
        StringBuffer ret = new StringBuffer();
        ret.append(lhs);
        //ret.append(" ->");
        for(String sym : rhs) {
            ret.append(" "+sym);
        }
        return ret.toString();
    }
}
/** Representation of a context-free grammar. */
class Grammar {
    Set<String> terminals = new LinkedHashSet<String>();
    Set<String> nonterminals = new LinkedHashSet<String>();
    Set<Production> productions = new LinkedHashSet<Production>();
    String start;

    public boolean isTerminal(String s) {
        return terminals.contains(s);
    }
    public boolean isNonTerminal(String s) {
        return nonterminals.contains(s);
    }
    public List<String> syms() {
        List<String> ret = new ArrayList<String>();
        ret.addAll(terminals);
        ret.addAll(nonterminals);
        return ret;
    }
}
class Error extends RuntimeException {
    public Error(String s) { super(s); }
}
class Util {
    public static String readLine(Scanner in, String msg) {
        if(!in.hasNextLine()) throw new Error(msg+" but input file ended");
        return in.nextLine();
    }
    public static int toInt(String line, String msg) {
        try {
            return new Integer(line);
        } catch(NumberFormatException e) {
            throw new Error("Expecting "+msg+" but the line is not a number:\n"+line);
        }
    }
    public static Grammar readGrammar(Scanner in) {
        Grammar grammar = new Grammar();
        String line = readLine(in, "Expecting number of non-terminals");
        int nterm = toInt(line, "number of non-terminals");
        for(int i = 0; i < nterm; i++) {
            String term = readLine(in, "Expecting a non-terminal").intern();
            if(!grammar.terminals.add(term))
                throw new Error("Duplicate terminal: "+term);
        }

        line = readLine(in, "Expecting number of non-terminals");
        int nnonterm = toInt(line, "number of non-terminals");
        for(int i = 0; i < nnonterm; i++) {
            String nonterm = readLine(in, "Expecting a non-terminal").intern();
            if(!grammar.nonterminals.add(nonterm))
                throw new Error("Duplicate non-terminal: "+nonterm);
            if(grammar.isTerminal(nonterm))
                throw new Error("Cannot be both terminal and non-terminal: "+nonterm);
        }

        grammar.start = readLine(in, "Expecting start symbol").intern();
        if(!grammar.nonterminals.contains(grammar.start)) throw new Error(
            "Start symbol "+grammar.start+" was not declared as a non-terminal.");

        line = readLine(in, "Expecting number of productions");
        int nprods = toInt(line, "number of productions");
        for(int i = 0; i < nprods; i++) {
            Production prod = readProduction(readLine(in, "Expecting production"), grammar);
            if(!grammar.productions.add(prod)) {
                throw new Error("Duplicate production: "+prod);
            }
        }
        if(in.hasNextLine()) {
            System.err.println("Warning: extra input lines after grammar; maybe your production count is wrong.");
        }
        return grammar;
    }
    public static Production readProduction(String line, Grammar grammar) {
        Scanner s = new Scanner(line);
        if(!s.hasNext()) throw new Error("Empty line instead of a production");
        String lhs = s.next().intern();
        if(!grammar.isNonTerminal(lhs)) throw new Error("Symbol "+lhs+" was not declared as a non-terminal, but appears on the LHS of production "+line);
        List<String> rhs = new ArrayList<String>();
        while(s.hasNext()) {
            String sym = s.next().intern();
            if(!grammar.isNonTerminal(sym) && !grammar.isTerminal(sym)) {
                throw new Error("Symbol "+sym+" is not a part of the grammar");
            }
            rhs.add(sym);
        }
        return Production.v(lhs, 
                (String[]) rhs.toArray(new String[rhs.size()]));
    }
    static String checkIndent(String line, int indent) {
        for(int i = 0; i < indent; i++) {
            if(line.length() <= i) throw new Error("Expecting production but got empty line.");
            if(line.charAt(i) != ' ') throw new Error("Production "+line.substring(i)+" should be indented "+indent+" space(s), but it is indented "+i+" spaces");
        }
        if(line.length() <= indent) throw new Error("Expecting production but got empty line.");
        if(line.charAt(indent) == ' ') throw new Error("Production "+line+" should be indented "+indent+" spaces, but it is indented more than that.");
        return line.substring(indent);
    }
    static void printGrammar(Grammar grammar) {
        System.out.println("Terminals:");
        for(String s : grammar.terminals) {
            System.out.println("   "+s);
        }
        System.out.println();

        System.out.println("Nonterminals:");
        for(String s : grammar.nonterminals) {
            System.out.println("   "+s);
        }
        System.out.println();

        System.out.println("Start Symbol:");
        System.out.println("   "+grammar.start);
        System.out.println();

        System.out.println("Production Rules:");
        for(Production s : grammar.productions) {
            System.out.println("   "+s);
        }
    }
    static void writeGrammar(Grammar grammar) {
        System.out.println(grammar.terminals.size());
        for(String s : grammar.terminals) {
            System.out.println(s);
        }
        System.out.println(grammar.nonterminals.size());
        for(String s : grammar.nonterminals) {
            System.out.println(s);
        }
        System.out.println(grammar.start);
        System.out.println(grammar.productions.size());
        for(Production s : grammar.productions) {
            System.out.println(s);
        }
    }
}

