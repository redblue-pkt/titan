package gs.filmon.tithek;

import gs.filmon.data.*;
import gs.filmon.rest.Client;

import java.io.*;
import java.util.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author Grzegorz Swiatek
 */
public class Create {
	private static final String LINK_PREFIX = "http://openaaf.dyndns.tv/mediathek/filmon/";
	private static final String MENU_LINK_PREFIX = "http://openaaf.dyndns.tv/mediathek/menu/";
	private static final String MENU = "FilmOn";
	private static final String SUB_DIR = "streams";

	private static AtomicInteger totalChannels = new AtomicInteger();

	public static void main(String[] args) {
		if (args.length !=1 && args.length != 2) {
			System.err.println("Usage: java gs.filmon.tithek.Create <filmon-out-dir> [<threads: 1..16 (default 1)>]");
			System.exit(-1);
		}

		int threads = 1;

		if (args.length == 2) {
			try {
				threads = Integer.parseInt(args[1]);
			} catch (NumberFormatException e) {
				System.err.println("Argument is not a number: " + args[1]);
			}

			if (threads < 1) {
				threads = 1;
			} else if (threads > 16) {
				threads = 16;
			}
		}

		try {
			File dir = new File(args[0]);

			if (!dir.exists()) {
				dir.mkdirs();
			} else {
				if (!dir.isDirectory()) {
					System.err.println(args[0] + " is not a directory");
					System.exit(-2);
				}
			}

			long start = System.currentTimeMillis();

			Client client = new Client();

			List<Group> groups = client.readList(Group.class, "groups");

			if (groups != null) {
				System.out.println("Found " + groups.size()  + " channel groups");

				writeMainList(dir, groups);

				File streamDir = new File(dir, SUB_DIR);

				if (!streamDir.exists()) {
					streamDir.mkdir();
				}

				ChannelWalker walker = new ChannelWalker();

				if (threads > 1) {
					ExecutorService service = Executors.newFixedThreadPool(8);
					CountDownLatch countDown = new CountDownLatch(groups.size());

					for (Group g : groups) {
						service.submit(new GroupHandler(walker, streamDir, g, countDown));
					}

					countDown.await();
					service.shutdown();
				} else {
					for (Group g : groups) {
						writeGroupList(walker, client, streamDir, g);
					}
				}

				System.out.println("Store All and A..Z");
				walker.store(dir, streamDir);

			} else {
				System.err.println("Could not read channel groups!");
			}



			long stop = System.currentTimeMillis();

			System.out.println(String.valueOf(totalChannels.intValue()) + " channels created");

			System.out.println("Processing time (ms): " + (stop - start));
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}

	private static void writeMainList(File dir, List<Group> groups) throws IOException {
		try (BufferedWriter w = new BufferedWriter(new FileWriter(new File(dir, "filmon.category.list")), 8196)) {

			for (Group group: groups) {
				w.write(group.getName());
				w.write("#");
				w.write(LINK_PREFIX);
				w.write(SUB_DIR);
				w.write("/group_");
				w.write(group.getId());
				w.write(".list#");
				w.write(group.getLargeLogoUri());
				w.write("#");
				w.write(SUB_DIR);
				w.write("_group_");
				w.write(group.getId());
				w.write(".png");
				w.write("#");
				w.write(MENU);
				w.write("#0");
				w.newLine();
				w.flush();
			}
		}
	}

	private static void writeGroupList(ChannelWalker channelWalker, Client client, File dir,  Group groupSummary) throws IOException {
		System.out.println("handle group: " + groupSummary.getName() + " (id=" + groupSummary.getId() + ")");
		try (BufferedWriter w = new BufferedWriter(new FileWriter(new File(dir, "group_" + groupSummary.getId() + ".list")), 2048)) {

			Group groupDetail = client.read(Group.class, "group/" + groupSummary.getId());

			if (groupDetail != null) {
				List<Channel> channels = groupDetail.getChannels();

				if (channels != null) {
					System.out.println("\t" + channels.size() + " channels found");

					for (Channel channel: channels) {
						gs.filmon.data.Channel channelDetail = client.read(gs.filmon.data.Channel.class, "channel/" + channel.getId());

						if (channelDetail != null) {
							Stream stream = channelDetail.getLowQualityStream();

							if (stream != null) {
								totalChannels.incrementAndGet();
								channelWalker.channelAdded(channelDetail);

								w.write(channel.getTitle());
								w.write("#");
								w.write(stream.getUrl());
								w.write("/");
								w.write(stream.getName());
								w.write("#");
								w.write(channel.getBigLogo());
								w.write("#");
								w.write(SUB_DIR);
								w.write("_channel_");
								w.write(channel.getId());
								w.write(".png");
								w.write("#");
								w.write(MENU);
								w.write("#2");
								w.newLine();
								w.flush();
							}
						}
					}
				}
			}
		}
	}

	static class GroupHandler implements Runnable {
		private static final ThreadLocal<Client> client = new ThreadLocal<Client>() {

			@Override
			protected Client initialValue() {
				try {
					return new Client();
				} catch (IOException e) {
					throw new RuntimeException("Unable to get FilmOn client.");
				}
			}
		};

		private File dir;
		private Group groupSummary;
		private CountDownLatch countDown;
		private ChannelWalker channelWalker;

		GroupHandler(ChannelWalker channelWalker, File dir, Group groupSummary, CountDownLatch countDown) {
			this.dir = dir;
			this.groupSummary = groupSummary;
			this.countDown = countDown;
			this.channelWalker = channelWalker;
		}

		public void run() {
			try {
				writeGroupList(channelWalker, client.get(), dir, groupSummary);
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				if (countDown != null) {
					countDown.countDown();
				}
			}
		}
	}

	static class ChannelWalker {
		static final char[] letters = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
				'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
			};

		List<Channel> allChannels = Collections.synchronizedList(new ArrayList<Channel>());
		Map<Character, List<Channel>> channelMap = Collections.synchronizedMap(new HashMap<Character, List<Channel>>());

		public ChannelWalker() {
			for (char c: letters) {
				channelMap.put(c, Collections.synchronizedList(new ArrayList<Channel>()));
			}
		}

		public void channelAdded(Channel channel) {
			char c = channel.getTitle().trim().toLowerCase().charAt(0);

			allChannels.add(channel);

			List<Channel> channels = channelMap.get(c);

			if (channels != null) {
				channels.add(channel);
			} else {
				System.err.println("Not stored in map: " + channel.getTitle());
			}
		}

		public void store(File dir, File streamDir) throws IOException {
			writeAll(streamDir);


			List<Character> used = new ArrayList<>();

			for (char c: letters) {
				List<Channel> channels = channelMap.get(c);

				if (!channels.isEmpty()) {
					writeAZ(streamDir, c, channels);
					used.add(c);
				}
			}

			writeAZIndex(dir, used);
		}

		private void writeAll(File dir) throws IOException {
			Collections.sort(allChannels);

			try (BufferedWriter w = new BufferedWriter(new FileWriter(new File(dir, "filmon.all-sorted.list")), 16384)) {
				Stream stream;

				for (Channel channel: allChannels) {
					stream = channel.getLowQualityStream();
					w.write(channel.getTitle());
					w.write("#");
					w.write(stream.getUrl());
					w.write("/");
					w.write(stream.getName());
					w.write("#");
					w.write(channel.getBigLogo());
					w.write("#");
					w.write(SUB_DIR);
					w.write("_channel_");
					w.write(channel.getId());
					w.write(".png");
					w.write("#");
					w.write(MENU);
					w.write("#2");
					w.newLine();
					w.flush();
				}
			}
		}

		private void writeAZIndex(File dir, List<Character> letters) throws IOException {

			try (BufferedWriter w = new BufferedWriter(new FileWriter(new File(dir, "filmon.a-z.list")), 16384)) {

				for (Character c: letters) {
					w.write(c);
					w.write("#");
					w.write(LINK_PREFIX);
					w.write(SUB_DIR);
					w.write("/filmon.");
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

		private void writeAZ(File dir, char c, List<Channel> channels) throws IOException {
			Collections.sort(channels);

			try (BufferedWriter w = new BufferedWriter(new FileWriter(new File(dir, "filmon." + c + ".list")), 16384)) {
				Stream stream;

				for (Channel channel: channels) {
					stream = channel.getLowQualityStream();
					w.write(channel.getTitle());
					w.write("#");
					w.write(stream.getUrl());
					w.write("/");
					w.write(stream.getName());
					w.write("#");
					w.write(channel.getBigLogo());
					w.write("#");
					w.write(SUB_DIR);
					w.write("_channel_");
					w.write(channel.getId());
					w.write(".png");
					w.write("#");
					w.write(MENU);
					w.write("#2");
					w.newLine();
					w.flush();
				}
			}
		}
	}
}
