// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// auxlib.java
// Auxlib has system access functions that can be used by other
//   classes to print appropriate messages and keep track of
//   the program name and exit codes.  It assumes it is being run
//   from a jar and gets the name of the program from the classpath.
//   Can not be instantiated.

import static java.lang.System.*;
import static java.lang.Integer.*;

public final class auxlib{
   public static final String PROGNAME =
                 basename (getProperty ("java.class.path"));
   public static final int EXIT_SUCCESS = 0;
   public static final int EXIT_FAILURE = 1;
   public static int exitvalue = EXIT_SUCCESS;

   //
   // private ctor - prevents class from new instantiation.
   //
   private auxlib () {
      throw new UnsupportedOperationException ();
   }

   //
   // basename - strips the dirname and returns only the basename.
   //            See:  man -s 3c basename
   //
   public static String basename (String pathname) {
      if (pathname == null || pathname.length () == 0) return ".";
      String[] paths = pathname.split ("/");
      for (int index = paths.length - 1; index >= 0; --index) {
         if (paths[index].length () > 0) return paths[index];
      }
      return "/";
   }


   //
   // Functions:
   //    whine   - prints a message with a given exit code.
   //    warn    - prints a stderr message and sets the exit code.
   //    die     - calls warn then exits.
   // Combinations of arguments:
   //    objname - name of the object to be printed (optional)
   //    message - message to be printed after the objname,
   //              either a Throwable or a String.
   //
   public static void whine (int exitval, Object... args) {
      exitvalue = exitval;
      err.printf ("%s", PROGNAME);
      for (Object argi : args) err.printf (": %s", argi);
      err.printf ("%n");
   }
   public static void warn (Object... args) {
      whine (EXIT_FAILURE, args);
   }
   public static void die (Object... args) {
      warn (args);
      exit ();
   }

   //
   // usage_exit - prints a usage message and exits.
   //
   public static void usage_exit (String optsargs) {
      exitvalue = EXIT_FAILURE;
      err.printf ("Usage: %s %s%n", PROGNAME, optsargs);
      exit ();
   }

   //
   // exit - calls exit with the appropriate code.
   //        This function should be called instead of returning
   //        from the main function.
   //
   public static void exit () {
      System.exit (exitvalue);
   }

   //
   // identity - returns the default Object.toString value
   //            Useful for debugging.
   //
   public static String identity (Object object) {
      return object == null ? "(null)"
           : object.getClass().getName() + "@"
           + toHexString (identityHashCode (object));
   }

}
