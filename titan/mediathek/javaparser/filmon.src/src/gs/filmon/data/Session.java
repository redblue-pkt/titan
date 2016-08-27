package gs.filmon.data;

import org.json.JSONObject;

/**
 * @author Grzegorz Swiatek
 */
public class Session implements JsonHandler {
	private static final String SESSION_KEY = "session_key";

	private String sessionKey;

	public Session() {

	}

	public String getSessionKey() {
		return sessionKey;
	}


	@Override
	public void read(JSONObject object) {
		try {
			if (object.has(SESSION_KEY)) {
				sessionKey = object.getString(SESSION_KEY);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
