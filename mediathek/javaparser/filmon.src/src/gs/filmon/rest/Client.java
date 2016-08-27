package gs.filmon.rest;

import gs.filmon.data.JsonHandler;
import gs.filmon.data.Session;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;


/**
 * FilmOn specific REST client
 *
 * @author Grzegorz Swiatek
 */
public class Client {
	private static final String CONTENT_TYPE = "application/json";
	private static final String HOST = "www.filmon.com";
	private static final String API = "/tv/api/";

	private final String urlPrefix;
	private String sessionKey;


	public Client() throws IOException {
		StringBuilder sb = new StringBuilder(100);

		sb.append("http://").append(HOST).append(':').append(80).append(API);

		this.urlPrefix = sb.toString();

		init(0);
	}

	private static <T extends JsonHandler> T getObjectFromJson(Class<T> className, String data) throws IOException {
		if (data == null || data.isEmpty()) {
			return null;
		}

		try {
			JSONObject object = new JSONObject(data);

			T handler = className.newInstance();
			handler.read(object);

			return handler;
		} catch (InstantiationException|IllegalAccessException e) {
			e.printStackTrace(); // TODO: log
		} catch (JSONException e) {
			System.err.println("JSON Exception caught for data: " + data); // TODO: log
			e.printStackTrace();
		}

		return null;
	}

	private static <T extends JsonHandler> List<T> getListFromJson(Class<T> className, String data) throws IOException {
		if (data == null) {
			return null;
		}

		try {
			JSONArray array = new JSONArray(data);

			List<T> result = new ArrayList<>();

			for (int i = 0; i< array.length(); i++) {
				try {
					T handler = className.newInstance();
					handler.read(array.getJSONObject(i));
					result.add(handler);

				} catch (InstantiationException|IllegalAccessException e) {
					e.printStackTrace(); // TODO: log
				}
			}

			return result;
		} catch (JSONException e) {
			System.err.println("JSON Exception caught for data: " + data); // TODO: log
			e.printStackTrace();
		}

		return null;
	}


	/**
	 * Reads a JSON object using the GET request.
	 *
	 * @param className The class of the new object to read
	 * @param urlSuffix URL suffix to be appended in the HTTP request
	 * @param <T>
	 * @return
	 * @throws IOException
	 */
	public <T extends JsonHandler> T read(Class<T> className, String urlSuffix) throws IOException {
		return getObjectFromJson(className, read(urlSuffix, 0));
	}

	private <T extends JsonHandler> T read(Class<T> className, String urlSuffix, int level) throws IOException {
		return getObjectFromJson(className, read(urlSuffix, level));
	}
	/**
	 * Reads a list of JSON objects using GET request.
	 *
	 * @param className The class of new objects to read
	 * @param urlSuffix URL suffix to be appended in the HTTP request
	 * @param <T>
	 * @return
	 * @throws IOException
	 */
	public <T extends JsonHandler> List<T> readList(Class<T> className, String urlSuffix) throws IOException {
		return getListFromJson(className, read(urlSuffix, 0));
	}

	private void init(int level) throws IOException {
		Session session = read(Session.class, "init");

		if (session != null) {
			sessionKey = session.getSessionKey();
		}
	}

	private String read(String urlSuffix, int level) throws IOException {
		String url;

		if (urlPrefix != null && !urlSuffix.startsWith("http://")) {
			url = urlPrefix + urlSuffix;
		} else {
			url = urlSuffix;
		}

		if (sessionKey != null) {
			url = url + "?session_key=" + sessionKey;
		}

		URL u = new URL(url);

		HttpURLConnection conn = null;

		try {
			conn = (HttpURLConnection) u.openConnection();
			conn.setRequestMethod("GET");
			conn.setUseCaches(false);
			conn.setRequestProperty("Accept", CONTENT_TYPE);
			conn.setConnectTimeout(30000);
			conn.setReadTimeout(30000);

			int code = conn.getResponseCode();

			if (code == 200) {
				StringBuilder sb = new StringBuilder(8192);

				BufferedReader r = new BufferedReader(new InputStreamReader(conn.getInputStream()));

				String line = r.readLine();

				while (line != null) {
					sb.append(line).append('\n');
					line = r.readLine();
				}

				return sb.toString();
			} else if (code == 410) {
				if (level < 3) {
					sessionKey = null;
					init(level + 1);
					return read(urlSuffix, level + 1);
				}
			}
		} finally {
			if (conn != null) {
				conn.disconnect();
			}
		}

		return null;
	}
}
