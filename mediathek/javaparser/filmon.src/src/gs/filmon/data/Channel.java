package gs.filmon.data;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Grzegorz Swiatek
 */
public class Channel implements Comparable<Channel>, JsonHandler {
	private static final String ID = "id";
	private static final String TITLE = "title";
	private static final String LOGO = "logo";
	private static final String BIG_LOGO = "big_logo";
	private static final String STREAMS = "streams";

	private String id;
	private String title;
	private List<Stream> streams;
	private String logo;
	private String bigLogo;

	public Channel() {

	}

	public String getId() {
		return id;
	}

	public String getTitle() {
		return title;
	}

	public List<Stream> getStreams() {
		return streams;
	}

	public String getLogo() {
		return logo;
	}

	public String getBigLogo() {
		return bigLogo;
	}


	@Override
	public String toString() {
		return "id=" + id + ", title=" + title;
	}

	public Stream getLowQualityStream() {
		Stream res = null;

		if (streams != null) {
			for (Stream s: streams) {
				if (s.getQuality() == Stream.Quality.LOW) {
					res = s;
					break;
				}
			}
		}

		return res;
	}

	@Override
	public int compareTo(Channel other) {
		return title.trim().toLowerCase().compareTo(other.getTitle().toLowerCase());
	}

	@Override
	public void read(JSONObject object) {
		if (object == null) {
			return;
		}

		try {
			if (object.has(ID)) {
				try {
					id = object.getString(ID);
				} catch (JSONException e) {
					id = String.valueOf(object.getInt(ID));
				}
			}

			if (object.has(TITLE)) {
				title = object.getString(TITLE);
			}

			if (object.has(LOGO)) {
				logo = object.getString(LOGO);
			}

			if (object.has(BIG_LOGO)) {
				bigLogo = object.getString(BIG_LOGO);
			}

			if (object.has(STREAMS)) {
				JSONArray array = object.getJSONArray(STREAMS);

				if (array.length() > 0) {
					streams = new ArrayList<>();
				}

				for (int i = 0; i< array.length(); i++) {
					Stream stream = new Stream();

					try {
						stream.read(array.getJSONObject(i));
						streams.add(stream);
					} catch (Exception e) {
						e.printStackTrace(); // TODO: log
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace(); // TODO: log
		}
	}
}
