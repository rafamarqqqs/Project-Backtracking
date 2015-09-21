
import java.io.*;
import java.util.Random;
import java.util.LinkedList;
import java.io.Writer;
import java.io.FileWriter;

public class createMaps {
	private static LinkedList<State> states = new LinkedList<State>();
	private static int size;

	public static void main(String[] args) {

		if(args.length < 1){
			System.out.println("Usage: createMaps <amount of states>");
			return;
		}
		
		size = Integer.parseInt(args[0]);
		
		if(size <= 0){
			System.out.println("Number of states must be greater than 0.");
			return;	
		}

		Random rand = new Random();
		PrintWriter pw;

		try{
			pw = new PrintWriter(new BufferedWriter(new FileWriter("random_country.in")));

			for(int i = 0; i < size; i++)
				states.add(new State(new Integer(i).toString()));

			for(State s : states){
				int adjNumber = rand.nextInt(size/10) + 1;

				for(int j = 0; j < adjNumber; j++){
					State aux = getState(s);
					s.addAdjacent(aux);
					aux.addAdjacent(s);
				}
			}

			pw.println(new Integer(size).toString());

			for(State s : states){
				pw.print(s.getName() + ":");

				if(s.getStates().size() == 0)
					pw.print(" .");

				for(State t : s.getStates()){
					pw.print(" " + t.getName());

					if(s.getStates().listIterator(s.getStates().indexOf(t) + 1).hasNext())
						pw.print(",");
					else
						pw.print(".");
				}

				pw.println();
			}

			pw.close();

		}
		catch(IOException e){
			System.out.println("failed");
		}
	}

	private static State getState(State state){
		Random r = new Random();
		String n = new Integer(r.nextInt(size)).toString();

		while(n.equals(state.getName()) && !state.hasState(new Integer(n).toString()))
			n = new Integer(r.nextInt(size)).toString();

		return states.get(Integer.parseInt(n));
	}

	public static class State {
		private String name;
		private LinkedList<State> adj;

		public State(String name){
			this.name = name;
			adj = new LinkedList<State>();
		}

		public boolean hasState(String s){
			return adj.contains(s);
		}

		public void addAdjacent(State s){
			if(!adj.contains(s))
				adj.add(s);
		}

		public String getName(){
			return name;
		}

		public LinkedList<State> getStates(){
			return adj;
		}
	}
}