package gs.filmon.data;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Grzegorz Swiatek
 */
public class Group implements JsonHandler {
	private static final String ID = "id";
	private static final String GROUP_ID = "group_id";
	private static final String NAME = "name";
	private static final String TITLE = "title";
	private static final String LOGO_URI_LARGE = "logo_148x148_uri";
	private static final String LOGO_URI = "logo_uri";
	private static final String CHANNELS = "channels";

	private String id;
	private String name;
	private String largeLogoUri;
	private List<Channel> channels;

	public Group() {

	}

	public String getId() {
		return id;
	}

	public String getName() {
		return name;
	}

	public String getLargeLogoUri() {
		return largeLogoUri;
	}

	public List<Channel> getChannels() {
		return channels;
	}

	public void read(JSONObject object) {
		if (object == null) {
			return;
		}

		try {
			if (object.has(ID)) {
				id = object.getString(ID);
			} else if (object.has(GROUP_ID)) {
				id = object.getString(GROUP_ID);
			}

			if (object.has(LOGO_URI_LARGE)) {
				largeLogoUri = object.getString(LOGO_URI_LARGE);
			} else if (object.has(LOGO_URI)) {
				largeLogoUri = object.getString(LOGO_URI);
			}

			if (object.has(NAME)) {
				name = object.getString(NAME);
			} else if (object.has(TITLE)) {
				name = object.getString(TITLE);
			}

			if (object.has(CHANNELS)) {
				JSONArray array = object.getJSONArray(CHANNELS);

				if (array.length() > 0) {
					channels = new ArrayList<>();
				}

				for (int i = 0; i < array.length(); i++) {
					try {
						int val = array.getInt(0);
						break; // we have only channel id
					} catch (JSONException e) { }

					Channel channel = new Channel();

					try {
						channel.read(array.getJSONObject(i));
						channels.add(channel);
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
