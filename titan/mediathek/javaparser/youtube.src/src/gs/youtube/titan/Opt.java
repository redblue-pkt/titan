package gs.youtube.titan;

import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;
import org.kohsuke.args4j.Option;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Grzegorz Swiatek
 */
public class Opt {

	public static final class Locale {
		private final String lang;
		private final String country;

		public Locale(String s) {
			int pos = s.indexOf('_');

			if (pos != -1) {
				lang = s.substring(0, pos).toLowerCase();
				country = s.substring(pos + 1).toLowerCase();
			} else {
				lang = s.toLowerCase();
				country = s.toLowerCase();
			}
		}

		public String getLang() {
			return lang;
		}

		public String getCountry() {
			return country;
		}

		@Override
		public String toString() {
			return lang + "_" + country;
		}

		@Override
		public boolean equals(Object o) {
			if (this == o) {
				return true;
			}

			if (o instanceof Locale) {
				Locale l = (Locale) o;
				return lang.equals(l.getLang()) && country.equals(l.getCountry());
			}

			return false;
		}

		@Override
		public int hashCode() {
			return toString().hashCode();
		}
	}

	@Option(name = "-key", usage = "API key", required = true)
	private String apiKey;

	@Option(name = "-loc", usage = "locale as ISO language code with optional country code (e.g. de_at, en_gb); default: de_de")
	private String locale = "de_de";

	@Option(name = "-d", usage = "directory where results are stored; default: youtube")
	private String dir = "youtube";

	@Option(name = "-s", usage = "search string, when specified we perform a video search")
	private String search;

	@Option(name = "-thumbnail", usage = "fetch thumbnails to local directory")
	private boolean fetchThumbnails;

	public Opt(String[] args) {
		CmdLineParser parser = new CmdLineParser(this);

		try {
			parser.parseArgument(args);
		} catch (CmdLineException e) {
			System.err.println(e.getMessage());
			parser.printUsage(System.err);
			System.exit(-1);
		}
	}

	public String getApiKey() {
		return apiKey;
	}

	public String getLocale() {
		return locale;
	}

	public List<Locale> getLocaleList() {

		List<Locale> res = new ArrayList<>();

		if (locale != null) {
			String[] entries = locale.split(",");

			for (String s : entries) {
				res.add(new Locale(s.trim()));
			}
		}

		return res;
	}

	public String getDir() {
		return dir;
	}

	public String getSearch() {
		return search;
	}

	public boolean isFetchThumbnails() {
		return fetchThumbnails;
	}
}
