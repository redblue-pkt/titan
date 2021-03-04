package gs.youtube.titan;

import com.google.api.services.youtube.model.Video;
import gs.youtube.Client;

import java.io.*;
import java.net.URL;
import java.net.URLConnection;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author Grzegorz Swiatek
 */
public class TiThek {

	private static final String LINK_PREFIX = "http://openaaf.dyndns.tv/mediathek/youtube/";
	//private static final String LINK_PREFIX = "http://192.168.1.40:8080/mediathek/youtube/";
	private static final String MENU_LINK_PREFIX = "http://openaaf.dyndns.tv/mediathek/menu/";
	private static final String MENU = "YouTube";
	private static final String SUB_DIR = "streams";
	private static final char[] LETTERS = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
			'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
	};

	private static Hashtable<String, String> urlMap = new Hashtable<>();
	private static final AtomicInteger thumbnailHits = new AtomicInteger();
	private static final AtomicInteger urlHits = new AtomicInteger();
	private static final AtomicInteger thumbnailErrors = new AtomicInteger();
	private static final AtomicInteger urlErrors = new AtomicInteger();

	private static void store(File dir, String loc, List<Client.Category> streamsByCategory) throws IOException {
		if (!dir.exists()) {
			dir.mkdirs();
		} else {
			if (!dir.isDirectory()) {
				System.err.format("%s is not a directory%n", dir.getAbsolutePath());
				System.exit(-2);
			}
		}

		writeMainList(dir, loc, streamsByCategory);

		File streamDir = new File(dir, SUB_DIR);

		if (!streamDir.exists()) {
			streamDir.mkdir();
		}


		for (Client.Category cat : streamsByCategory) {
			writeList(streamDir, "group_" + cat.getId() + "_" + loc + ".list", cat.getVideos());
		}


	}

	private static void writeMainList(File dir, String loc, List<Client.Category> categories) throws IOException {
		try (BufferedWriter w = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(dir, "youtube.category.list." + loc)), "utf-8"), 8196)) {

			for (Client.Category cat : categories) {
				w.write(cat.getName());
				w.write("#");
				w.write(LINK_PREFIX);
				w.write(SUB_DIR);
				w.write("/group_");
				w.write(cat.getId());
				w.write("_");
				w.write(loc);
				w.write(".list");
				w.write("#");
				w.write(LINK_PREFIX + "img/cat_"+ cat.getId() + ".png"); // logo
				w.write("#");
				w.write(SUB_DIR);
				w.write("_group_");
				w.write(cat.getId());
				w.write(".png");
				w.write("#");
				w.write(MENU);
				w.write("#0");
				w.newLine();
				w.flush();
			}
		}
	}

	private static void storeAll(File dir, List<Client.StreamInfo> all) throws IOException {
		Collections.sort(all);

		File streamDir = new File(dir, SUB_DIR);

		if (!streamDir.exists()) {
			streamDir.mkdir();
		}

		writeList(streamDir, "youtube.all-sorted.list", all);

		Map<Character, ArrayList<Client.StreamInfo>> streamIndex = new HashMap<>();

		for (char c : LETTERS) {
			streamIndex.put(c, new ArrayList<Client.StreamInfo>());
		}

		for (Client.StreamInfo s : all) {
			try {
				char first = s.getTitle().trim().toLowerCase().charAt(0);
				streamIndex.get(first).add(s);
			} catch (Throwable e) {

			}
		}

		List<Character> used = new ArrayList<>();

		for (char c: LETTERS) {
			List<Client.StreamInfo> indexStreams = streamIndex.get(c);

			if (!indexStreams.isEmpty()) {
				writeList(streamDir, "youtube." + c + ".list", indexStreams);
				used.add(c);
			}
		}

		writeAZIndex(dir, used);
	}

	private static void writeAZIndex(File dir, List<Character> letters) throws IOException {
		try (BufferedWriter w = new BufferedWriter(new FileWriter(new File(dir, "youtube.a-z.list")), 16384)) {

			for (Character c: letters) {
				w.write(c);
				w.write("#");
				w.write(LINK_PREFIX);
				w.write(SUB_DIR);
				w.write("/youtube.");
				w.write(c);
				w.write(".list");
				w.write("#");
				w.write(MENU_LINK_PREFIX);
				w.write(c);
				w.write(".jpg");
				w.write("#");
				w.write(c);
				w.write(".jpg");
				w.write("#");
				w.write(MENU);
				w.write("#0");
				w.newLine();
				w.flush();
			}
		}
	}

	private static void writeList(File dir, String fileName, List<Client.StreamInfo> streams) throws IOException {
		try (BufferedWriter w = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(dir, fileName)), "utf-8"), 16384)) {

			for (Client.StreamInfo stream: streams) {
				if (stream.getStreamUrl() == null || stream.getThumbnailUrl() == null) {
					continue;
				}
				w.write(stream.getTitle());
				w.write("#");
				w.write(stream.getStreamUrl());
				w.write("#");

				if (stream.isThumbnailFetched()) {
					w.write(LINK_PREFIX + SUB_DIR + "/img/" + stream.getId() + stream.getThumbnailExtension());
				} else {
					w.write(stream.getThumbnailUrl());
				}
				w.write("#");
				w.write(SUB_DIR);
				w.write("_channel_");
				w.write(stream.getId());
				w.write(".jpg");
				w.write("#");
				w.write(MENU);
				w.write("#2");
				w.newLine();
				w.flush();
			}
		}
	}

	static class Resolver implements Runnable {

		private final Client.StreamInfo streamInfo;
		private final CountDownLatch latch;
		private final boolean fetchThumbnail;
		private final File imgDir;

		Resolver(Client.StreamInfo streamInfo, boolean fetchThumbnail, File imgDir, CountDownLatch latch) {
			this.streamInfo = streamInfo;
			this.fetchThumbnail = fetchThumbnail;
			this.imgDir = imgDir;
			this.latch = latch;
		}

		@Override
		public void run() {
			try {
				String resolvedUrl = urlMap.get(streamInfo.getId());

				if (resolvedUrl == null) {
					resolvedUrl = Client.getStreamUrlFromVideoId(streamInfo.getId());

					if (resolvedUrl != null) {
						urlMap.put(streamInfo.getId(), resolvedUrl);
					} else {
						urlErrors.incrementAndGet();
					}
				} else {
					urlHits.incrementAndGet();
				}

				streamInfo.setStreamUrl(resolvedUrl);

				if (fetchThumbnail && streamInfo.getThumbnailUrl() != null) {

					File localImg = new File(imgDir, streamInfo.getId() + streamInfo.getThumbnailExtension());

					if (localImg.exists()) {
						thumbnailHits.incrementAndGet();
						streamInfo.setThumbnailFetched(true);
					} else {
						URL url = new URL(streamInfo.getThumbnailUrl());
						URLConnection conn = url.openConnection();
						conn.setConnectTimeout(15000);
						conn.setReadTimeout(10000);

						final int bufSize = 8192;
						try (FileOutputStream out = new FileOutputStream(new File(imgDir, streamInfo.getId() + streamInfo.getThumbnailExtension())); InputStream in = conn.getInputStream()) {
							byte buf[] = new byte[bufSize];

							int count = in.read(buf);

							while (count != -1) {
								if (count > 0) {
									out.write(buf, 0, count);
								}

								count = in.read(buf);
							}

							streamInfo.setThumbnailFetched(true);
						}
					}
				}
			} catch (IOException e) {
				System.err.format("Unable to get thumbnail for video id=%s%n", streamInfo.getId());
				thumbnailErrors.incrementAndGet();
			} finally {
				latch.countDown();
			}
		}
	}


	public static void main(String[] args) {
		Opt opt = new Opt(args);

		long start = System.currentTimeMillis();
		try {
			if (opt.getSearch() != null) {
				System.err.format("search started for %s%n", opt.getSearch());
				List<Video> videos = Client.search(opt.getApiKey(), opt.getSearch());
				long time = time  = (System.currentTimeMillis() - start) / 1000L;
				System.err.format("found %d video (processing time=%ds)%n", videos.size(), time);
				List<Client.StreamInfo> streams = Client.getStreams(videos);

				for (Client.StreamInfo stream : streams) {
					System.out.format("%s#%s#%s#%n", stream.getTitle(), stream.getThumbnailUrl(), stream.getStreamUrl());
				}
			} else {

				System.err.format("started%n");

				ExecutorService threadPool = Executors.newFixedThreadPool(16);

				List<Opt.Locale> locales = opt.getLocaleList();
				List<Client.StreamInfo> all = new ArrayList<>(4000);

				Map<Opt.Locale, List<Client.Category>> foundVideos = new HashMap();

				for (Opt.Locale locale : locales) {
					int count = 0;
					List<Client.Category> categories = Client.getPopularStreams(opt.getApiKey(), locale.getCountry(), locale.getLang());

					for (Client.Category cat : categories) {
						all.addAll(cat.getVideos());
						count += cat.getVideos().size();
					}

					System.err.format("found %d videos for %s (processing time=%ds)%n", count, locale, ((System.currentTimeMillis() - start) / 1000L));
					foundVideos.put(locale, categories);
				}

				System.err.format("total # videos: %d (processing time=%ds)%n", all.size(), ((System.currentTimeMillis() - start) / 1000L));

				CountDownLatch latch = new CountDownLatch(all.size());

				System.err.format("submit resolve URLs tasks%n");

				File imgDir = null;

				if (opt.isFetchThumbnails()) {
					File streamDir = new File(opt.getDir(), SUB_DIR);

					if (!streamDir.exists()) {
						streamDir.mkdirs();
					}

					imgDir = new File(streamDir, "img");

					if (!imgDir.exists()) {
						imgDir.mkdirs();
					}
				}

				for (Client.StreamInfo streamInfo : all) {
					threadPool.submit(new Resolver(streamInfo, opt.isFetchThumbnails(), imgDir, latch));
				}

				System.err.format("wait until all resolver finished (processing time=%ds)%n", ((System.currentTimeMillis() - start) / 1000L));

				while (!latch.await(30, TimeUnit.SECONDS)) {
					long count = latch.getCount();

					float resolved = ((all.size() - count) * 100) / all.size();
					System.err.format("%.1f%% resolved (processing time=%ds)%n", resolved, ((System.currentTimeMillis() - start) / 1000L));
				}
				long time  = (System.currentTimeMillis() - start) / 1000L;
				System.err.format("URL resolving finished (processing time=%ds)%n", time);

				for (Opt.Locale loc : foundVideos.keySet()) {
					System.err.format("store %s%n", loc.toString());

					try {
						store(new File(opt.getDir()), loc.toString(), foundVideos.get(loc));
					} catch (IOException e) {
						System.err.format("I/O error in store %s: %s%n", loc.toString(), e.getMessage());
					}
				}

				System.err.format("store all%n");

				try {
					storeAll(new File(opt.getDir()), all);
				} catch (IOException e) {
					System.err.format("I/O error in storeAll: %s%n", e.getMessage());
				}

				threadPool.shutdown();

				time  = (System.currentTimeMillis() - start) / 1000L;
				System.err.format("thumbnails: hits=%d, errors=%d%n", thumbnailHits.intValue(), thumbnailErrors.intValue());
				System.err.format("stream URLs: hits=%d, errors=%d%n", urlHits.intValue(), urlErrors.intValue());
				System.err.format("finished in %ds%n", time);
			}
		} catch (IOException e) {
			System.err.println("I/O error: " + e.getMessage());
			System.exit(-1);
		} catch (Throwable e) {
			System.err.println("error: " + e.toString());
			System.exit(-1);
		}
	}
}
