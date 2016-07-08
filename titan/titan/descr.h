int descrambler_set_key1(struct dvbdev* node, int index, int parity, unsigned char *data)
{
	
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	struct ca_descr_data d;

	if (descrambler_open())
	{
		d.index = index;
		d.parity = parity;
		d.data_type = CA_DATA_KEY;
		d.length = 32;
		d.data = data;

		if (ioctl(desc_fd, CA_SET_DESCR_DATA, &d))
			printf("CA_SET_DESCR_DATA\n");
	}
	descrambler_close();
}
