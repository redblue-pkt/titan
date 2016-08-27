package gs.filmon.data;


import org.json.JSONObject;

/**
 * @author Grzegorz Swiatek
 */
public class Stream implements JsonHandler {
	public static enum Quality {
		HIGH("high"),
		LOW("low");

		private final String value;

		private Quality(String value) {
			this.value = value;
		}

		public String getValue() {
			return value;
		}

		public static Quality fromValue(String val) {
			Quality res = null;
			for (Quality q: Quality.values()) {
				if (q.getValue().equalsIgnoreCase(val)) {
					res = q;
					break;
				}
			}

			return res;
		}
	}

	private static final String ID  = "id";
	private static final String QUALITY = "quality";
	private static final String URL = "url";
	private static final String NAME = "name";
	private static final String WATCH_TIMEOUT = "watch-timeout";

	private int id;
	private Quality quality;
	private String url;
	private String name;
	private int watchTimeout;

	public Stream() {

	}

	public int getId() {
		return id;
	}

	public Quality getQuality() {
		return quality;
	}

	public String getUrl() {
		return url;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getWatchTimeout() {
		return watchTimeout;
	}

	@Override
	public String toString() {
		return "quality=" + quality + ", name=" + name + ", timeout=" + watchTimeout + "\n\turl=" + url;
	}

	@Override
	public void read(JSONObject object) {
		if (object == null) {
			return;
		}

		try {
			if (object.has(ID)) {
				id = object.getInt(ID);
			}

			if (object.has(QUALITY)) {
				quality = Quality.fromValue(object.getString(QUALITY));
			}

			if (object.has(URL)) {
				url = object.getString(URL);
			}

			if (object.has(NAME)) {
				name = object.getString(NAME);
			}

			if (object.has(WATCH_TIMEOUT)) {
				watchTimeout = object.getInt(WATCH_TIMEOUT);
			}
		} catch (Exception e) {
			e.printStackTrace(); // TODO: log
		}
	}
}
