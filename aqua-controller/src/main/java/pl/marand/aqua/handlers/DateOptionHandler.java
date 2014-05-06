package pl.marand.aqua.handlers;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;
import org.kohsuke.args4j.OptionDef;
import org.kohsuke.args4j.spi.OneArgumentOptionHandler;
import org.kohsuke.args4j.spi.Setter;

public class DateOptionHandler extends OneArgumentOptionHandler<Date> {

	private String[] dateFormats = {
			"dd/MM/yyyy HH:mm:ss",
			"HH:mm"
	};
	
	public DateOptionHandler(CmdLineParser parser, OptionDef option, Setter<? super Date> setter) {
		super(parser, option, setter);
	}

	@Override
	protected Date parse(String argument) throws NumberFormatException, CmdLineException {
		
		Date newDate = null;
		
		if ("now".equals(argument.toLowerCase())) {	
			newDate = new Date();
		} else {
			
			for(String format: dateFormats){
				SimpleDateFormat sdf = new SimpleDateFormat(format);
				try {
					newDate = sdf.parse(argument);
					break;
				} catch (ParseException e) {
					//Try another date format
					continue;
				}
			}
		}	
		return newDate;
	}
}
