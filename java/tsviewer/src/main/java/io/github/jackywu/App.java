package io.github.jackywu;

import java.io.File;
import java.util.HashMap;
import java.util.Arrays;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.LineIterator;
import org.apache.commons.lang3.ArrayUtils;

public class App
{

    public static HashMap<String, String> zip(String[] L1, String[] L2) {
        var map = new HashMap<String, String>();
        for (int i=0; i<L1.length; i++) {
            map.put(L1[i], L2[i]);
        }
        return map;
    }

    public static HashMap<String, String> filter(HashMap<String, String> map, String[] list) {
        var newMap = new HashMap<String, String>();
        for (String key : map.keySet()) {
            if (ArrayUtils.contains(list, key)) {
                newMap.put(key, map.get(key));
            }
        }
        return newMap;
    }

    public static void main( String[] args )
    {
        var options = new Options();
        options.addOption("f", "file", true, "file to read");
        options.addOption("l", "line", true, "show X line");
        options.addOption("c", "column", true, "only show column x,y,z");

        String file = null;
        CommandLine cli = null;
        String[] column = null;
        int line = 3;

        var parser = new DefaultParser();
        var formatter = new HelpFormatter();

        try {
            cli = parser.parse(options, args);
        } catch(Exception e){
            System.err.println("parse args error: " + e.getMessage());
            System.exit(1);
        }

        if (cli.hasOption("f")) {
            file = cli.getOptionValue("f");
        } else {
            // System.err.println("missing arg -f/--file");
            formatter.printHelp("TSVViewer", options, true);
            System.exit(1);
        }

        if (cli.hasOption("l")) {
            try {
                line = Integer.parseInt(cli.getOptionValue("l"));
            } catch(Exception e){
                System.err.println("parse args error: " + e.getMessage());
                System.exit(1);
            }
        }

        if (cli.hasOption("c")) {
            try {
                column = cli.getOptionValue("c").split(",", -1);
            } catch(Exception e){
                System.err.println("parse args error: " + e.getMessage());
                System.exit(1);
            }
        }

        LineIterator it = null;

        int num = 0;
        String[] header = null;
        String[] segment = null;

        var mapper = new ObjectMapper();

        try {
            it = FileUtils.lineIterator(new File(file), "UTF-8");
            while (it.hasNext()) {
                final String row = it.nextLine();
                segment = row.split("\\s", -1);
                num++;
                if (num == 1) {
                    header = segment;
                } else {
                    var map = App.zip(header, segment);
                    System.out.println(Arrays.toString(column));
                    if (column != null) {
                        map = App.filter(map, column);
                    }
                    String json = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(map);
                    System.out.println(json);
                }
                if (num == line+1) {
                    System.exit(0);
                }
            }
        } catch(Exception e){
            System.err.println("read file error: " + e.getMessage());
            System.exit(1);
        } finally {
            it.close();
        }
    }
}
