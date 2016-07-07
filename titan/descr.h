int descrambler_set_key1(struct dvbdev* node, int index, int parity, unsigned char *data)
{
	
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	struct ca_descr_data d;

	d.index = index;
	d.parity = parity;
	d.data_type = CA_DATA_KEY;
	d.length = 16;
	d.data = data;

	if (ioctl(node->fd, CA_SET_DESCR_DATA, &d))
		printf("CA_SET_DESCR_DATA\n");

	d.index = index;
	d.parity = parity;
	d.data_type = CA_DATA_IV;
	d.length = 16;
	d.data = data + 16;

	if (ioctl(node->fd, CA_SET_DESCR_DATA, &d))
		printf("CA_SET_DESCR_DATA\n");

	return 0;
}
