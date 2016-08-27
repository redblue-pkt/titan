package gs.youtube;

import com.google.api.client.http.HttpRequest;
import com.google.api.client.http.HttpRequestInitializer;
import com.google.api.client.http.HttpTransport;
import com.google.api.client.http.javanet.NetHttpTransport;
import com.google.api.client.json.JsonFactory;
import com.google.api.client.json.jackson2.JacksonFactory;
import com.google.api.client.util.Joiner;
import com.google.api.services.youtube.YouTube;
import com.google.api.services.youtube.model.*;

import javax.net.ssl.HttpsURLConnection;
import java.io.*;
import java.net.URL;
import java.util.*;

/**
 * REST API Client for Youtube V3.
 *
 * @author Grzegorz Swiatek
 */
public class Client {
	public static final class Category implements Comparable<Category>{
		private final String id;
		private final String name;
		private List<StreamInfo> videos;

		public Category(VideoCategory cat) {
			id = cat.getId();
			name = cat.getSnippet().getTitle();
		}


		public String getId() {
			return id;
		}

		public String getName() {
			return name.replace("#", " ");
		}

		@Override
		public boolean equals(Object o) {
			if (this == o) {
				return true;
			}

			if (o instanceof Category) {
				Category cat = (Category) o;

				return id.equals(cat.getId()) && name.equals(cat.getName());
			}

			return false;
		}

		@Override
		public int hashCode() {
			return id.hashCode();
		}

		@Override
		public int compareTo(Category other) {
			return name.trim().toLowerCase().compareTo(other.getName().toLowerCase());
		}

		public List<StreamInfo> getVideos() {
			return videos;
		}

		public void setVideos(List<StreamInfo> videos) {
			this.videos = videos;
		}
	}

	public static final class StreamInfo implements Comparable<StreamInfo> {
		private final String id;
		private final String title;
		private final String thumbnailUrl;
		private String streamUrl;
		private boolean thumbnailFetched;

		public StreamInfo(Video video) {
			this.id = video.getId();
			this.title = (video.getSnippet().getTitle() != null ? video.getSnippet().getTitle() : " ");

			ThumbnailDetails thumbnails = video.getSnippet().getThumbnails();

			if (thumbnails != null) {
				Thumbnail thumbnail = thumbnails.getHigh();

				if (thumbnail == null) {
					thumbnail = thumbnails.getMedium();

					if (thumbnail == null) {
						thumbnail = thumbnails.getStandard();
					}
				}

				if (thumbnail != null) {
					this.thumbnailUrl = thumbnail.getUrl();
				} else {
					this.thumbnailUrl = null;
				}
			} else {
				this.thumbnailUrl = null;
			}
		}

		public String getId() {
			return id;
		}

		public String getTitle() {
			return title.replaceAll("#", " ");
		}

		public String getThumbnailUrl() {
			return thumbnailUrl;
		}

		public String getStreamUrl() {
			return streamUrl;
		}

		public void setStreamUrl(String streamUrl) {
			this.streamUrl = streamUrl;
		}

		public boolean isThumbnailFetched() {
			return thumbnailFetched;
		}

		public void setThumbnailFetched(boolean thumbnailFetched) {
			this.thumbnailFetched = thumbnailFetched;
		}

		public String getThumbnailExtension() {
			if (thumbnailUrl != null) {
				int pos = thumbnailUrl.lastIndexOf('.');

				if (pos != -1) {
					return thumbnailUrl.substring(pos).toLowerCase();
				} else {
					return ".jpg";
				}
			}

			return null;
		}

		@Override
		public boolean equals(Object o) {
			if (this == o) {
				return true;
			}

			if (o instanceof StreamInfo) {
				StreamInfo si = (StreamInfo) o;

				return id.equals(si.getId()) && title.equals(si.getTitle());
			}

			return false;
		}

		@Override
		public int hashCode() {
			return id.hashCode();
		}

		@Override
		public int compareTo(StreamInfo other) {
			return title.trim().toLowerCase().compareTo(other.getTitle().toLowerCase());
		}
	}

	private static final String APP = "titan";
	private static final long MAX_RESULTS = 50;
	private static final HttpTransport HTTP_TRANSPORT = new NetHttpTransport();
	private static final JsonFactory JSON_FACTORY = new JacksonFactory();

	public static GuideCategoryListResponse guideCategories(String apiKey, String region, String lang) throws IOException {
		YouTube youtube = new YouTube.Builder(HTTP_TRANSPORT, JSON_FACTORY, new HttpRequestInitializer() {
			public void initialize(HttpRequest request) throws IOException {
			}
		}).setApplicationName("titan").build();

		YouTube.GuideCategories.List categories = youtube.guideCategories().list("id,snippet");
		categories.setKey(apiKey);
		categories.setRegionCode(region);
		categories.setHl(lang);

		return categories.execute();
	}

	public static List<VideoCategory> videoCategories(String apiKey, String region, String lang) throws IOException {
		YouTube youtube = new YouTube.Builder(HTTP_TRANSPORT, JSON_FACTORY, new HttpRequestInitializer() {
			public void initialize(HttpRequest request) throws IOException {
			}
		}).setApplicationName(APP).build();

		YouTube.VideoCategories.List categories = youtube.videoCategories().list("id,snippet");
		categories.setKey(apiKey);
		categories.setRegionCode(region);
		categories.setHl(lang);

		return categories.execute().getItems();
	}


	public static Map<VideoCategory, List<Video>> mostPopularByCategory(String apiKey, String region, String lang) throws IOException {
		YouTube youtube = new YouTube.Builder(HTTP_TRANSPORT, JSON_FACTORY, new HttpRequestInitializer() {
			public void initialize(HttpRequest request) throws IOException {
			}
		}).setApplicationName(APP).build();

		Map<VideoCategory, List<Video>> res = new HashMap<>();

		for (VideoCategory cat : videoCategories(apiKey, region, lang)) {
			YouTube.Videos.List listVideosRequest = youtube.videos().list("id,snippet");
			listVideosRequest.setKey(apiKey);
			listVideosRequest.setRegionCode(region).setChart("mostPopular").setVideoCategoryId(cat.getId());
			listVideosRequest.setMaxResults(MAX_RESULTS);
			res.put(cat, listVideosRequest.execute().getItems());
		}

		return res;
	}

	public static List<Category> getPopularStreams(String apiKey, String region, String lang) throws IOException {
		YouTube youtube = new YouTube.Builder(HTTP_TRANSPORT, JSON_FACTORY, new HttpRequestInitializer() {
			public void initialize(HttpRequest request) throws IOException {
			}
		}).setApplicationName(APP).build();

		List<Category> res = new ArrayList<>();

		for (VideoCategory cat : videoCategories(apiKey, region, lang)) {
			Category c = new Category(cat);

			YouTube.Videos.List listVideosRequest = youtube.videos().list("id,snippet");
			listVideosRequest.setKey(apiKey);
			listVideosRequest.setRegionCode(region).setChart("mostPopular").setVideoCategoryId(cat.getId());
			listVideosRequest.setMaxResults(MAX_RESULTS);

			c.setVideos(getStreams(listVideosRequest.execute().getItems()));

			if (!c.getVideos().isEmpty()) {
				res.add(c);
			}
		}

		return res;
	}

	public static List<StreamInfo> getStreams(List<Video> videos) {
		List<StreamInfo> streams = new ArrayList<>();

		for (Video v : videos) {
			StreamInfo info = new StreamInfo(v);
			streams.add(info);
			/*String url = getStreamUrlFromVideoId(v.getId());

			if (url != null) {
				info.setStreamUrl(url);
				streams.add(info);
			} else {
				System.err.format("[%s] Unable to get streamUrl URL for video with id=%s%n", Thread.currentThread().getName(), v.getId());
			}*/
		}

		return streams;
	}

	public static Map<VideoCategory, List<StreamInfo>> getStreams(Map<VideoCategory, List<Video>> videosByCategory) {

		Map<VideoCategory, List<StreamInfo>> streams = new HashMap<>();

		for (VideoCategory cat : videosByCategory.keySet()) {
			streams.put(cat, getStreams(videosByCategory.get(cat)));
		}

		return streams;
	}

	private static String getStreamUrlFromContent(String line) {

		String res = null;

		if (line == null) {
			return res;
		}

		String token =  "url_encoded_fmt_stream_map\":\"";

		int pos = line.indexOf(token);

		if (pos != -1) {
			line = line.substring(pos + token.length());

			pos = line.indexOf('"');

			if (pos != -1) {
				line = line.substring(0, pos);
			}
		}

		List<String> parts = new ArrayList<>();

		String delim = "\\u0026";

		pos = line.indexOf(delim);

		while (pos != -1) {
			parts.add(line.substring(0, pos));
			line = line.substring(pos + delim.length());
			pos = line.indexOf(delim);
		}

		parts.add(line);

		String urlToken = "url=";

		for (String s : parts) {
			if (s.startsWith(urlToken)) {
				res = s.substring(urlToken.length());
				break;
			}
		}

		if (res != null) {
			res = res.replaceAll("%3A", ":").replaceAll("%2F", "/").replaceAll("%3F", "?").replaceAll("%26", "&").replaceAll("%3D", "=").replaceAll("%25", "%");
		}

		return res;
	}

	public static String getStreamUrlFromVideoId(String id) {

		String streamUrl;
		String streamLine = null;

		try {
			URL url = new URL("https://www.youtube.com/watch?v=" + id);
			HttpsURLConnection conn = (HttpsURLConnection) url.openConnection();
			conn.setConnectTimeout(15000);
			conn.setReadTimeout(10000);

			try (BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()))) {
				String line = br.readLine();

				while (line != null) {

					if (line.contains("videoplayback")) {
						streamLine = line;
						break;
					}

					line = br.readLine();
				}
			}
		} catch (IOException e) {
			System.err.format("Unable to get video content for id=%s%n", id);
		}

		streamUrl = getStreamUrlFromContent(streamLine);

		return streamUrl;
	}

	public static List<Video> search(String apiKey, String query) throws IOException {
		YouTube youtube = new YouTube.Builder(HTTP_TRANSPORT, JSON_FACTORY, new HttpRequestInitializer() {
			public void initialize(HttpRequest request) throws IOException {
			}
		}).setApplicationName(APP).build();


		// Define the API request for retrieving search results.
		YouTube.Search.List search = youtube.search().list("id,snippet");

		search.setKey(apiKey);
		search.setQ(query);

		search.setType("video");

		// To increase efficiency, only retrieve the fields that the
		// application uses.
		search.setFields("items(id/kind,id/videoId,snippet)");
		search.setMaxResults(MAX_RESULTS);

		// Call the API and print results.
		SearchListResponse searchResponse = search.execute();
		List<SearchResult> searchResultList = searchResponse.getItems();

		System.err.format("search result %d%n", searchResultList.size());

		List<String> videoIds = new ArrayList<>();

		if (searchResultList != null) {
			Joiner stringJoiner = Joiner.on(',');
			String videoId = stringJoiner.join(videoIds);

			YouTube.Videos.List listVideosRequest = youtube.videos().list("id,snippet,recordingDetails").setId(videoId);
			listVideosRequest.setKey(apiKey);
			VideoListResponse listResponse = listVideosRequest.execute();

			return listResponse.getItems();
		}

		return null;
	}
}
