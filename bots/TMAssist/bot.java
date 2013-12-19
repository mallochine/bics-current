/*
 * 	 TMAssist Project, Version 1.0
 * 	 Copyright (C) 2008 John Nahlen (john.nahlen@gmail.com)
 *  
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * @author John Nahlen (johnthegreat)
 * @version 1.0 [WORKING]
 * @since October 3, 2008
 * @deprecated This code is highly un-optimized and very ugly. For example, some commands issue multiple 
 * 		qtells to the server when someone requests information from it. To optimize it would take a bit 
 * 		of work and testing.
 */
package TMAssist;
import java.io.*;
import java.net.*;
import java.util.*;
import java.text.SimpleDateFormat;


public class bot
{
	   public static void main(String[] args) throws Exception
	   {		   
		   checkForScheduledEvents(); // every 15 minutes it checks for scheduled events 
			SimpleDateFormat fvar = new SimpleDateFormat("MMddyy");
			fname = fvar.format(new Date());
			File myFile  = new File("C:\\Documents and Settings\\John\\Desktop\\logs\\log" + fname + ".txt");
			file = myFile;
			if (!file.exists()) { file.createNewFile(); } else {  } 
			FileWriter myPrinter = new FileWriter(file,true);
			printer = myPrinter;
		   privateAuthorization = new ArrayList<String>();
		   privateAuthorization.add("volcano"); privateAuthorization.add("johnthegreat");
		   noAutoGreet = new ArrayList<String>(); noAutoGreet.add("volcano"); noAutoGreet.add("CDay");
		   
		if (run == true)
	    {
		   connect();
		   Thread t = new Thread(new Runnable() { public void run() { try { while(true) { consoleInput(); } } catch (Exception ex) {} } });
		   t.setDaemon(true);
		   t.start();
				
		   while(true) 
	        {
			   
		       	String input = read(prompt);
		       	if (input.indexOf(prompt) > -1) { input = input.intern(); input = input.replaceFirst(input.substring(input.indexOf(prompt)),""); input = input.intern(); } // [attempt to] supress prompt
		       	if (input.indexOf(" tells you: ") != -1) 
		       	{
		       		myUsername = name(input,true);
		       	}
		       	if (input.contains(" if you have questions/problems with this tourney.")) { input = "[supressed]\n"; } 
		       	if (input.indexOf(":Listed: ") != -1) { input = "[supressed]\n"; } 
		 	   	if (input.indexOf(":TMs:") != -1) { getAllManagers(input.replaceAll("\n","")); input = "[supressed]\n"; } // get manager list and supress the rest
		 	    if (input.indexOf("tm(s)") != -1)
		 	    {
		 	    if (input.indexOf("logged in.") != -1) // get manager list and supress the rest
		 	    	{
		 	    	input = input.replaceAll("\r","");
		 	    	input = input.replaceAll("\n","");
		 	    	input = input.replaceAll("tm\\(s\\)","");
		 	    	input = input.replaceAll("logged in.","");
		 	    	input = input.replaceAll(":Or ask your question in channel 49.","");
		 	    	getAllManagers(input);
		 	    	input = "[supressed]\n";
		 	    	}
		 	    }
		 	    if (input.indexOf("*qtell ") != -1) { input = "[supressed]\n"; } // supress qtell command-feedback
		       	if (input.indexOf(":Statistics for") != -1) { input = "[supressed]\n"; } // supress part of "td f"
		       	//if (!input.startsWith(":") && input.indexOf("Variable settings of ") != -1) { getInterface(myUsername); input = "[supressed]\n"; } // supress part of "var <user>"
		       	if (input.indexOf(":| Blitz:      |") != -1) { input = "[supressed]\n"; } // supress part of "td f"
		       	if ((input.indexOf(":| Total:      |") != -1) && (lastOutputtedMessage.equals("td f"))) { /* getTourneyPlayedCount(input); */ } // supress part of "td f"
		        if (input.indexOf(":\r") != 1) { input = input.replaceAll(":\r",""); }
		       	if ((input.indexOf(":Last Played: ") != 1) && (input.indexOf(":Manager Level: ") != -1)) { input = input.substring(input.indexOf(":Manager Level: "),input.length()); }
		       	if (input.indexOf("**ANNOUNCEMENT** from ") != -1) { input = "[supressed]\n"; } // supress server-wide announcements
		       	
		       	SimpleDateFormat hourMinute = new SimpleDateFormat("[MM/dd/yy kk:mm]");
			    String time = hourMinute.format(new Date());
		       	if (!input.equals("[supressed]\n")) 
		       		{
		       		System.out.print(time + input);
		       		logConsole(printer,input,true);
		       		}
		       	

		    if (input.indexOf(":Present TM company: ") == 0) { parseTDZNotify(input); }
		    if (input.indexOf(":Notification: ") != -1) { parseTDPIN(input); }
		    //if (input.indexOf(" has joined tourney ") != -1) { logAllMamerUsers(input); }
	       	//if (input.indexOf(":Weekly Tournament Schedule") == 1) {  }
	       	//if (input.indexOf(":Manager Level: ") != -1) { levels(myUsername,getManagerLevel(myUsername,input)); }
	       	if (input.indexOf("The following message was received:") != -1) { fmessage(); }
	       	//if (input.indexOf(":mamer's tourney list:") != -1) { announce(input); }
	        if (input.indexOf("Notification: ") == 0) if (input.indexOf(" has arrived") != -1) { greet(input); }	
	        //if (input.indexOf("Finger of ") != -1) { isConnected(myUsername,message(input)); }
	        //if (input.indexOf(":mamer's tourney list:") != -1) { getOpenTourneys(input); }
	        //if (input.indexOf(":Listed: ") != -1) { if (input.indexOf("tourneys out of a total of ") != -1)
	        	//{ if (input.indexOf(", all open, all joinable.") != -1) { getOpenTourneys(input); /* do whatever */ } } }
	        
	    	if (input.indexOf(" tells you: ") != -1)
	    	{
	    	if (!myUsername.equals("<offender_name_here>")) // people that have been banned from the bot
	    	{
	    	 //if (input.indexOf("interface ") != -1) { output("var " + message(input)); }
	    	 //if (input.indexOf("credits") != -1) { credits(myUsername); }
	    	 //if (input.indexOf("autoannounce") != -1) { /*id = input.charAt("autoannounce".length()+2);*/ output("td lt -o"); } else
	         if (input.indexOf("exec ") != -1) { if (name(input,true).equals("johnthegreat")) { execute(name(input,true),message(input)); } } else
		     if (input.indexOf("requestlog") != -1) { readLog(name(input,true),message(input)); } else
	       	 if (input.indexOf("phelp") != -1) { phelp(name(input,true)); } else
	         if (input.indexOf("help") != -1) { help(name(input,true)); } else
	    	 //if (input.indexOf("mylevel") != -1) { qtell(myUsername,"Your user level for TMAssist(TD) is: " + myLevel + "/10"); } else 
		     //if (input.indexOf("level ") != -1) { output("td f " + message(input)); } else
		     //if (input.indexOf("level") != -1) { output("td f " + name(input,true)); } else
	         if (input.indexOf("showtm") != -1) { ShowTMs(name(input,true)); }
	    	}
	    	else qtell(myUsername,"TMAssist(TD): You have been banned from using this bot.");
	    	}
	    	     	 
	       	// DO NOT USE
	       	// UNDER ORDERS BY VOLCANO
	       	//if (input.indexOf("mamer TOURNEY INFO:") != -1) if (input.indexOf("has been opened!") != -1) { announceOnOpen(input); }
	        }
	    }
	    
		// FOR TESTING ONLY
	    if (run == false)
	    {
	    }
	   }
   
   private static boolean run = true;
   private static boolean publicNotify = false;
   private static String prompt = "fics%";
   private static boolean connected = false;
   private static ArrayList<String> noAutoGreet;
   private static ArrayList<String> privateAuthorization;
   private static FileWriter printer;
   private static String lastOutputtedMessage = "";

   private static String myUsername;
   private static String fname;
   private static File file;
   private static Socket socket;
   private static DataInputStream dataIn;
   private static OutputStream dataOut;
   public static String reminderMessage = "";
   private static ArrayList<String> OnlineManagers = new ArrayList<String>();

   public static void connect()
   {
       try{
	   socket = new Socket("freechess.org",5000);
       dataIn = new DataInputStream(socket.getInputStream());
       dataOut = socket.getOutputStream();
       
       System.out.print(read("login: "));
       String username = "TMAssist";
       String password = "";
       
       // username
       System.out.println(" " + username);
       write(username);
       // password 
       write(password);
       String mypassword = "";
       for(int i=0;i<password.length();i++)
       {
       mypassword += "*";
       }
       System.out.print(mypassword + "\n");
       
       System.out.println(read(prompt));
       write("iset nowrap 1");
       write("set interface TMAssist BOT v1.0");
       connected = true;
       } catch (Exception e) { System.err.println("connect() Failed."); System.exit(1); }
   }

	public static void greet(String name)
	{
       		name = name.replaceAll("Notification: ","");
       		name = name.replaceAll(" has arrived and isn't on your notify list.","");
       		name = name.replaceAll(" has arrived.","");
       		name = name.replaceAll("\n","");
       		name = name.replaceAll("\r","");
       		name = name.replaceAll("fics%","");
       		name = name.trim();
       		if (!noAutoGreet.contains(name))
       		{
       			if (publicNotify) output("It welcomes " + name + "!");
       			output("Tell " + name + " Hello!");
       		}
	}
   
   public static boolean write(String message)
   {    	   
	   try { dataOut.write((message + "\r\n").getBytes());
       dataOut.flush(); } catch (Exception e) { System.err.println("write() Failed."); System.exit(1); }
       return true;
   }


   public static String read(String prompt)
   {
       String result = "";
       try {
       result += (char)dataIn.read();
       while (!result.endsWith(prompt))
       {
           result += (char)(dataIn.read());
       }
       }
       catch(Exception se) { System.err.println("read() Failed."); write("logout"); connect(); }
       return result.replaceAll("\n","").trim();
   }
   
   
   public static String removeTitles(String name) 
   {
	   int parenIndex = name.indexOf("("); 
	   if (parenIndex == -1) 
	   { return name; } 
	   else 
	   { return name.substring(0,parenIndex); }
   }
   
    /*public static void logAllMamerUsers(String user)
	{
		try{
		// :mamer TOURNEY INFO: tiboloid(1590) has joined tourney #7 (seed: 2, score: 0.0); 4 players now!
		System.out.println(user);
		user = user.replaceAll(":mamer TOURNEY INFO: ","");
		user = user.substring(0,user.indexOf("("));
		System.out.println(user);
        FileWriter myFileWriter = new FileWriter("C:\\Documents and Settings\\John\\Desktop\\logs\\logTMCandidates.txt",true);
        myFileWriter.append(user + "\n");
        myFileWriter.flush();
		myFileWriter.close();
        } catch (IOException e) { System.out.println("FAILED"); }
	}*/
   
   /*public static void credits(String username)
   {
	   qtell(username,"TMAssist(TD): Credits (No particular order)");
	   qtell(username,"volcano(*)(TM)");
	   qtell(username,"johnthegreat(TM)");
	   qtell(username,"CDay");
	   qtell(username,"maciejg(SR)");
   }*/
   
   
   
   /*public static String getTourneyPlayedCount(String input)
   {
	   System.out.println(input);
	   //input = (input.substring(0,20)).trim();
	   return input;
   }*/
   
   public static void getOpenTourneys(String input)
   { 
	   input = input.replaceAll("-","");
	   input = input.replaceAll(":>: Joined;","");
	   input = input.replaceAll(":\\+: Joinable;","");
	   input = input.replaceAll(":<: Running;","");
	   input = input.replaceAll(":\\*: Kept;","");
	   input = input.replaceAll(":!: Kept with comment.","");
	   input = input.replaceAll("\\+","");
	   input = input.replaceAll("ID","");
	   input = input.replaceAll("Status","");
	   input = input.replaceAll("Manager","");
	   input = input.replaceAll("Type","");
	   input = input.replaceAll("Start Date","");
	   input = input.replaceAll("\r","");
	   input = input.replaceAll("\n","");
	   input = input.replaceAll(" ","");
	   System.out.println(input);
   }
   
   
   
   public static String name(String input,boolean stripTags)
   {
	   String name = "";
	   int pos = input.indexOf(" tells you: ");
	   if (pos != -1)
	   {
	   name = input.substring(0,pos);
	   if (stripTags == true) name = removeTitles(name);
	   }
	   return name.trim();
   }
   
   public static String message(String input)
   {
	   String message;
	   message = input.substring(input.indexOf("tells you: ")+("tells you: ".length()),(input.length()-1));
	   if (message.startsWith("make ")) message = message.replaceFirst("make ","");
	   if (message.startsWith("exec ")) message = message.replaceFirst("exec ","");
	   if (message.startsWith("requestlog ")) message = message.replaceFirst("requestlog ","");
	   if (message.startsWith("authorize ")) message = message.replaceFirst("authorize ","");
	   if (message.startsWith("deauthorize ")) message = message.replaceFirst("deauthorize ","");
	   if (message.startsWith("results ")) message = message.replaceFirst("results ","");
	   if (message.startsWith("vote")) message = message.replaceFirst("vote","");
	   if (message.startsWith("level ")) message = message.replaceFirst("level ","");
	   if (message.startsWith("interface ")) message = message.replaceFirst("interface ","");
	   if (message.endsWith("fics")) message = message.replaceAll("fics","");
	   return message; 
   }
   
   public static void execute(String username,String command)
   {
	 if (privateAuthorization.contains(removeTitles(username)))
	 {
		 output(command);
	 }
	 else
		 qtell(username,"TMAssist(TD): You are unauthorized to use this command.");
		 
   }
   
   
   public static boolean isTM(String name)
   {
	if (name.indexOf("(TM)") != -1) { return true; } else { return false; }
   }
   
   
   
   /*public static void announceOnOpen(String input)
   {
	   String cut = "";
	   cut = input;
	   cut = cut.replaceAll(":mamer TOURNEY INFO: ","");
	   //cut = cut.replaceAll("\\*\\*\\* ","");
	   cut = cut.replaceAll("Tourney #","");
	   cut = cut.replaceAll(" a ","");
	   cut = cut.replaceAll(" has been opened!","");
	   cut = cut.replaceAll(":Type \"tell mamer JoinTourney 7\" to join.","");
	   cut = cut.replaceAll(":Contact","");
	   cut = cut.replaceAll(" if you have questions/problems with this tourney.",",");
	   cut = cut.replaceAll(":Tourney #","");
	   cut = cut.replaceAll(" has been opened..","");
	   cut = cut.replaceAll(":","");
	   //cut = cut.replaceAll(cut.substring(cut.indexOf("***"),cut.lastIndexOf("\\*\\*\\*")),",");
	   System.out.println(cut);
	   StringTokenizer myCut = new StringTokenizer(cut,",");
	   
	   String tmp = myCut.nextToken();
	   int tournId = Integer.parseInt(tmp);
	   String tournType = myCut.nextToken().trim();
	   //String tournManager = myCut.nextToken();
	   output("tell 49 A " + tournType + " has been opened! Use \"mam j " + tournId + "\" to join.");
	   // For questions or problems, please contact " + tournManager + "."
   }*/
   
   /*public static void announce(String input)
   {
	   input = input.replaceAll("mamer's tourney list","");
	   input = input.replaceAll(":","");
	   input = input.replaceAll("\\+","");
	   input = input.replaceAll("\\*","");
	   input = input.replaceAll("> Joined","");
	   input = input.replaceAll("Joinable","");
	   input = input.replaceAll("Running","");
	   input = input.replaceAll("!Keptwithcomment","");
	   input = input.replaceAll("Kept","");
	   input = input.replaceAll("open","");
	   input = input.replaceAll("-","");
	   input = input.replaceAll(";","");
	   input = input.replaceAll("< ","");
 
	   input = input.replaceFirst("ID","");
	   input = input.replaceFirst("Status","");
	   input = input.replaceFirst("Manager","");
	   input = input.replaceFirst("Type","");
	   input = input.replaceFirst("Start Date","");
	   input = input.replaceAll("\\| ","");
	   input = input.replaceAll("\r","");
	   input = input.replaceAll("\n","");
	   input = input.replaceAll("\\.","");
	   input = input.replaceAll("  "," ");
	   input = input.replaceAll("\\|","");
	   input = input.trim();
	   input = input.replaceAll("   ",",");
	   //input = input.replaceAll(input.substring(0,input.indexOf()));
	   System.out.println("DEBUG [input]:\n" + input);
	   StringTokenizer announcer = new StringTokenizer(input,",");
	   String id = announcer.nextToken();
	   String manager = announcer.nextToken();
	   String type = announcer.nextToken();
	   System.out.println("tell 49 A " + type + " is open! Type 'td j " + id + "' to join. Contact " + manager.trim() + " for any problems.");

   }*/

   /*public static void listTournaments(String input,char id)
   {
	   String myOutput = "";
	   int x = 0;
	   myOutput = input;
	   x = myOutput.indexOf(" " + id + "    ");
	   System.out.println(x);
	   announce(myOutput);
   }*/
   
   public static void help(String username)
   {
	   //String QTELL = "TMAssist(TD) Help File:\\n";
	   qtell(username,"TMAssist(TD): See my finger notes for how to use me.");
	   //QTELL += "This bot will auto-notify all TMs about scheduled events.\\n";
	   //QTELL += "For problems, please contact johnthegreat(TM).\\n";
	   //qtell(username,QTELL);
   }
   
   
   
   public static void phelp(String username)
   {
	   if (privateAuthorization.contains(removeTitles(username)))
		{
		   qtell(username,"TMAssist(TD) [Private Features] Help File:");
		   qtell(username,"---");
		   qtell(username,"To request the console log on a certain date:");
		   qtell(username,"tell TMAssist requestlog <MMDDYY>");
		   qtell(username,"Example: tell TMAssist requestlog 101508");
		   qtell(username,"Recommended Alias: alias tmlog tell TMAssist requestlog $@");
		   qtell(username,"Notes: --");
		   qtell(username,"---");
		   qtell(username,"To execute a command through me:");
		   qtell(username,"tell TMAssist exec <command>");
		   qtell(username,"Example: tell TMAssist exec shout Hello, " + username + "!");
		   qtell(username,"Recommended Alias: alias tmexec Tell TMAssist exec $@");
		   qtell(username,"Notes: Abusers will be banned!");
		   qtell(username,"---");
		}
	   else
		   qtell(username,"TMAssist(TD): You are unauthorized to use this command.");
   }
   
   public static void fmessage()
   {
	   output("fmessage johnthegreat 1");
	   output("clearmessages *");
   }
     
   public static void output(String message)
   {
	   try { logConsole(printer,message,false); } catch (Exception e) { }
	   if (connected == true) 
	   { 
			   System.out.println(message); 
			   try { write(message); } catch(Exception e) { }
	   } 
	   else
	   {
		   System.out.println(message);
	   }
	   lastOutputtedMessage = message;
   }
   
   public static void qtell(String user,String message)
   {
	 output("qtell " + user + " " + message);  
   }
   
   public static String getAllManagers(String test)
   {
	   test = test.replaceAll(",","");
	   //test = test.replaceAll(":\n","");
	   test = test.replaceAll("\n","");
	   test = test.replaceFirst(",","");
	   test = test.replaceFirst(",","");
	   test = test.replaceFirst(",","");
	   test = test.replaceAll(" ",",");
	   test = test.replaceAll(":","");
	   test = test.replaceAll("TMs","");
	   test = test.replaceAll(":","");
	   test = test.replaceAll("Name","");
	   test = test.replaceAll("Status","");
	   test = test.replaceAll("Idle","");
	   test = test.replaceAll("Managing","");
	   test = test.replaceAll("In_tourney","");
	   test = test.replaceAll("Playing","");
	   test = test.replaceAll("Available","");
	   test = test.replaceAll("Off_duty","");
	   test = test.replaceAll("Programmer","");
	   test = test.replaceAll("time","");
	   test = test.replaceAll("mins","");
	   test = test.replaceAll("min","");
	   test = test.replaceAll("secs","");
	   test = test.replaceAll("sec","");
	   test = test.replaceAll("[0-9]","");
	   test = test.replaceAll("hrs","");
	   test = test.replaceAll("hr","");
	   test = test.replaceAll("]","");
	   test = test.replaceAll("\\[","");
	   test = test.replaceAll("#","");
	   test = test.replaceFirst("tm\\(s\\)loggedin.,Oraskyourquestioninchannel.","");
	   test = test.replaceFirst("tm\\(s\\)loggedin.,,Oraskyourquestioninchannel.","");
	   test = test.substring(0,test.length()-1);
	   
	   // DO-NOT-NOTIFY SECTION
	   test = test.replaceAll("Ludens","");
	   test = test.replaceAll("ThoBjo","");
	   test = test.replaceAll("SuperIntellect","");
	   
	   notifyAllManagers(test);
	   return test;
   }
   
   protected static boolean output = true;
   // false = System.out.println();
   // true = output();
   
   public static void notifyAllManagers(String input)
   {
	   StringTokenizer managers = new StringTokenizer(input,",");
	   String TM = "";
	   while(managers.hasMoreTokens())
	   {
		   TM = managers.nextToken();
		   TM = TM.replaceAll("[^a-zA-Z]","");
		   TM = TM.trim();
		   if (output == true || TM.equals("johnthegreat")) { if (!TM.equals("")) { output("tell " + TM + " " + reminderMessage); } }
		   if (output == false) { if (!TM.equals("")) { System.out.println("tell " + TM + " " + reminderMessage); } }
		   
	   }
   }
   
   public static String nextScheduled = "1800000"; // 30 mins: these are in milliseconds
   
   public static class scheduled extends TimerTask
   { 
	   String dayOfWeek = "";
	   String timeOfDay = "";
	   boolean done = false;
	   public void run() 
	   {
		  //try { remindOLD(); } catch (Exception e) { }
	      getNextEvent();
	   }
	   public void getNextEvent()
	   {
		   nextScheduled = "";
		   String tmp = "";
		   try
		   { 
			   BufferedReader my_check = new BufferedReader(
        		   new FileReader("C:\\Documents and Settings\\John\\Desktop\\logs\\logScheduleds.txt")); 
		   	while(true)
		   	{
			   SimpleDateFormat day = new SimpleDateFormat("EEE");
			   dayOfWeek = day.format(new Date());
			   SimpleDateFormat time = new SimpleDateFormat("kkmm");
			   timeOfDay = ("" + (Integer.parseInt(time.format(new Date()))-100)); // server time
			   while(my_check.ready() && !tmp.startsWith(dayOfWeek)) // skips all lines that are not today
			   { 
				   tmp = my_check.readLine(); // reads the line
				   System.out.print("");
			   }
			   System.out.println(dayOfWeek);
			   tmp = my_check.readLine();
			   String nextEvent = "";
			   String startTime = "";
			   while(my_check.ready() && tmp.startsWith(dayOfWeek))
			   { 
				   StringTokenizer tok = new StringTokenizer(tmp," ");
				   tok.nextToken();
				   startTime = tok.nextToken();
				   tmp = my_check.readLine(); // reads the line
			
			   int i=0;
			   while (i<10) 
			   { 
				   i++;
				   tmp = my_check.readLine(); // reads the line
				   StringTokenizer tok1 = new StringTokenizer(tmp," ");
				   tok1.nextToken();
				   startTime = tok1.nextToken();
				   
				   if (Integer.parseInt(startTime) > Integer.parseInt(timeOfDay)) { System.out.println("FINAL: " + tmp); nextEvent = tmp; break; }
				   
			   }
			   if (Integer.parseInt(startTime) > Integer.parseInt(timeOfDay)) { System.out.println("FINAL: " + tmp); nextEvent = tmp; break; }
			   }
			   StringTokenizer partOfLine = new StringTokenizer(nextEvent," ");
			   partOfLine.nextToken();
			   startTime = partOfLine.nextToken();
			   SimpleDateFormat serverTimeSDF = new SimpleDateFormat("kkmm");
		       int serverTime = (Integer.parseInt(serverTimeSDF.format(new Date()))-100);
		       int difference = (Integer.parseInt(startTime) - serverTime);
		       nextScheduled = startTime;
		       
		       if (difference > 40) { difference -= 40; }
		       System.out.println("DEBUG [startTime]: " + (startTime));
		       System.out.println("DEBUG [serverTime]: " + (serverTime));
		       System.out.println("DEBUG [difference]: " + (difference));
		       		       
		       //int openTime = (difference-15);
		       int openTime = difference; if (openTime > 15) { openTime -= 15; }
		       if (openTime < 0) { openTime = 0; }
			   System.out.println("The timer should go off in " + ((openTime)*60*1000) + " milliseconds.");
			   output("tell johnthegreat The timer should go off in " + (openTime) + " minute(s).");
		       if (openTime>60)
		       { 
		    	   int hrs = (openTime/100);
		    	   int mins = (openTime%100);
		    	   if (mins >= 60) { hrs++; mins%=60;}
		    	   output("tell johnthegreat The timer should go off in " + hrs + " hour" + ((hrs==1)?"":"s") + " and " + mins + " minute" + ((mins==1)?".":"s."));
		    	   output("tell volcano The timer should go off in " + hrs + " hour" + ((hrs==1)?"":"s") + " and " + mins + " minute" + ((mins==1)?".":"s."));
		       }

			   int i=0;
			   String eventType = "";
			   String eventNumber = partOfLine.nextToken();
			   System.out.println("DEBUG [eventNumber]: " + eventNumber);
			   while(i<3)
				   {
				   i++;
				   if (i==1) { eventType += ("" + partOfLine.nextToken()); }
				   else
				   { eventType += (" " + partOfLine.nextToken()); }
				   }
			   String eventManager = "";
			   if (partOfLine.hasMoreTokens()) 
			   	{ eventManager = partOfLine.nextToken(); }
			   else
			   	{ eventManager = ""; }
			   String x = eventManager;
			   //if (!x.equals("zh") && !x.equals("ato") && !x.equals("los") && !x.equals("sui") && !x.equals("fr") && !x.equals("bug")) 
			   if(x.startsWith("SS") || x.startsWith("Din") || x.startsWith("RR") || x.startsWith("KO"))
			   		{ eventType += (" " + eventManager); if (partOfLine.hasMoreTokens()) { eventManager = partOfLine.nextToken(); } }
			   else
				    { x = ""; }
			   
			   System.out.println("DEBUG [eventType]: " + eventType);
			   System.out.println("DEBUG [eventManager]: " + eventManager);
			   
			   bot mybot = new bot(); 
			   Timer Timer_MAIN = new Timer();
			   bot.notifyMainManager MAIN = mybot.new notifyMainManager();
			   Timer_MAIN.schedule(MAIN,openTime*60*1000);
			   MAIN.manager = eventManager;
			   MAIN.message = " Your scheduled event (" + eventType + ", Event #" + eventNumber + ") is scheduled to start in " + (difference-openTime) + " minute" + (((difference-openTime)!=1)?"s.":".");
			   			   
			   Timer Timer_ALT = new Timer();
			   bot.notifyAllManagers ALT = mybot.new notifyAllManagers();
			   int GoOff = 0;
			   if (openTime>15) { GoOff=(2+openTime); }
			   if (openTime<=15) { GoOff=1; }
			   Timer_ALT.scheduleAtFixedRate(ALT,GoOff*60*1000,2*60*1000);
			   ALT.timer = Timer_ALT;
			   ALT.startingIn = (difference-openTime);
			   if (openTime<=15) { ALT.startingIn+=2; }
			   reminderMessage = "A scheduled event (" + eventType.trim() + ", Event #" + eventNumber + ") is scheduled to start in " + (difference-openTime) + " minute" + (((difference-openTime)==1)?".":"s.");
			   
			   MAIN.ALTTimer = ALT.timer;
			   
			   my_check.close();
			   break;
			   }
		   } catch(Exception x) { }
	   }
   }
   
   public class notifyMainManager extends TimerTask
   {
	   String manager = "";
	   String message = "";
	   Timer ALTTimer = new Timer();
	   public void run()
	   {
		   if (output == true || manager.equals("johnthegreat")) 
		   { 
			   output("tell " + manager + " " + message);
			   String in = read(prompt);
			   if (in.startsWith(manager) && in.contains(" is not logged in."))
			   {
				   ALTTimer.schedule(new notifyAllManagers(),0);
			   }
		   } else
		   if (output == false) { System.out.println(message); }
	   }
   }
   
   public class notifyAllManagers extends TimerTask
   {
	   int startingIn = 0;
	   Timer timer = new Timer();
	   public void run()
	   {
		   startingIn = startingIn-2;
		   if (startingIn < 0) { timer.cancel(); }
		   if (startingIn > -1) 
		   { 
		   System.out.println("DEBUG [reminderMessage][1]:" + reminderMessage);
		   System.out.println("DEBUG [startingIn]:" + startingIn);
		   System.out.println("DEBUG [startingIn+2]:" + (startingIn+2));
		   reminderMessage = bot.replaceLast(reminderMessage,(bot.reverseString(""+(startingIn+2))),bot.reverseString(""+startingIn));
		   if (startingIn == 1) { bot.replaceLast(reminderMessage,"s.","."); }
		   //System.err.println(reminderMessage);
		   System.out.println("DEBUG [reminderMessage][2]:" + reminderMessage);
		   notifyAllTMs(reminderMessage);
		   //output("td showtm");
		   }
		   
	   }
   }
   
   public static void checkForScheduledEvents()
   {
	   Timer myTimer = new Timer();
	   scheduled check = new scheduled();
	   myTimer.scheduleAtFixedRate(check,1000*30,Long.parseLong(nextScheduled));
   }
   
   public static void nextScheduled(String username)
   {
	   qtell(username,
			   "The next scheduled event is:\\n"
			   
	   );
   }
   
	public static void logConsole(FileWriter x,String input,boolean flush) throws IOException
	{ 
		/*SimpleDateFormat fvar = new SimpleDateFormat("MMddyy");
		fname = fvar.format(new Date());
		File myFile  = new File("C:\\Documents and Settings\\John\\Desktop\\logs\\log" + fname + ".txt");
		file = myFile;
		if (!file.exists()) { file.createNewFile(); } else {  }*/
		
		SimpleDateFormat hourMinute = new SimpleDateFormat("[MM/dd/yy kk:mm]");
       	String time = hourMinute.format(new Date());	
		input = input.replaceAll("\r","");
		input = input.replaceAll("\n","");
       	x.append(time + input + "\r\n");
		if (flush == true) { x.flush(); }
		//x.close();
	}
	
	public static boolean readLog(String username,String date)
	{
		System.err.println(date);
		if (!privateAuthorization.contains(username)) { qtell(username,"TMAssist(TD): You are unauthorized to use this command."); return false; }
		try{
		File myFile = new File("C:\\Documents and Settings\\John\\Desktop\\logs\\log" + date.replaceAll("\n","").trim() + ".txt");
		if (!myFile.exists()) { qtell(username,"TMAssist(TD): I'm sorry, but the requested log file does not exist."); return false; }
		
		BufferedReader myReader = new BufferedReader(new FileReader("C:\\Documents and Settings\\John\\Desktop\\logs\\log" + date.replaceAll("\n","").trim() + ".txt"));
		String tmp = "";
		if (date.equalsIgnoreCase("UPDATE")) { tmp = "UPDATE"; } else
		if (date.equalsIgnoreCase("BotQA")) { tmp = "BotQA"; } else
		
		if (date.equalsIgnoreCase("Scheduleds")) { tmp = "Scheduleds"; } else
		{
		StringTokenizer myDate = new StringTokenizer(date,"/.");
		tmp = myDate.nextToken();
		if (tmp.length()>2) 
		 { tmp = tmp.substring(0,2).concat("/") + tmp.substring(2,4).concat("/") + tmp.substring(4,6); }
		}
		
		qtell(username,"Log for: " + tmp);
		qtell(username,"-----");
		String line = null;
		while (myReader.ready()) 
		{ 
			line = myReader.readLine(); 
			if (line.equals("")) line = "---";
			qtell(username,line);
		}
		qtell(username,"-----");
		myReader.close();
		} catch (Exception e) { System.err.println("readLog() Failed."); qtell(username,"An internal error has occured. Please contact johnthegreat immediately."); }
		return true;
	}
	
	/*public static String getManagerLevel(String username,String input) throws Exception
	{
	 String data = input.replaceAll(input.substring(0,input.indexOf(":Manager Level:")+":Manager Level:".length()),"");
	 data = data.replaceAll("\n","").trim();
	 return data;
	}*/

	   public static void consoleInput()
	   {
		   try {
		   BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		   String command = reader.readLine();
		   if (command.startsWith("!exec "))
		   {
			   command = command.replaceAll("!exec ","");
		   }
		   else
		   {
		   if (!command.equals("!exec"))
			   { write(command); } else { System.out.println("Blocked."); }
		   }
		   } catch (Exception e) { System.err.println("consoleInput() Failed."); }
	   }
	   
	   public static String reverseString(String str)
	   {
		   String newstr = "";
		   for(int i=str.length()-1;i>-1;i--)
		   {
			   newstr += str.charAt(i);
		   }
		   return newstr;
	   }
	   
	   public static String replaceLast(String str,String replace,String replaceWith)
	   {
		   str = reverseString(str);
		   str = str.replaceFirst(replace,replaceWith);
		   str = reverseString(str);
		   return str;
	   }
	   
	   public static void parseTDZNotify(String in)
	   {
		   in = in.substring(0,in.indexOf(":Present director company: "));
		   in = in.replaceFirst(":Present TM company:","");
		   in = in.replaceAll(" and ",",");
		   in = in.replaceAll(" ","");
		   in = in.replaceAll("\\.","");
		   StringTokenizer toks = new StringTokenizer(in,",");
		   while(toks.hasMoreTokens())
		   {
			   OnlineManagers.add(toks.nextToken());
		   }
	   }
	   
	   public static void parseTDPIN(String in)
	   {
		   String name = in.replaceFirst(":Notification: ","");
		   name = name.replaceAll("has departed.","");
		   name = name.replaceAll("has arrived.","");
		   name = name.replaceAll("\\(TM\\)","");
		   name = name.replaceAll("[^a-zA-Z]","");
		   if (in.contains("has departed")) { if (OnlineManagers.contains(name)) OnlineManagers.remove(name); }
		   if (in.contains("has arrived")) { if (!OnlineManagers.contains(name)) OnlineManagers.add(name); }
	   }
	   
	   public static void notifyAllTMs(String message)
	   {
		   ArrayList<String> avoid = new ArrayList<String>();
		   avoid.add("ThoBjo"); avoid.add("Ludens"); avoid.add("SuperIntellect");
		   for(int i=0;i<OnlineManagers.size();i++)
		   {
			   if (!avoid.contains(OnlineManagers.get(i)))
			   {
			   //qtell(OnlineManagers.get(i),"TMAssist(TD): " + message);
			   if (output == true) write("tell " + OnlineManagers.get(i) + " " + message); else
			   if (output == false) System.out.println("tell " + OnlineManagers.get(i) + " " + message);
			   }
		   }
	   }
	   
	   public static void ShowTMs(String username)
	   {
		   String QTELL = "TMAssist(TD):\\n-----\\n";
		   for(int i=0;i<OnlineManagers.size();i++)
		   {
			   QTELL += OnlineManagers.get(i)+"\\n";
		   }
		   QTELL += "-----";
		   qtell(username,QTELL);
	   }
}