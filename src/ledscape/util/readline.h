static ssize_t
readline(
	FILE * const file,
	char * const buf,
	const int max_len
)
{
	if (!fgets(buf, max_len, file))
		return -1;
	int len = strlen(buf);
	if (len >= max_len)
	{
		buf[max_len-1] = '\0';
		return max_len;
	}

	while (len > 0)
	{
		const char c = buf[len-1];
		if (!isspace(c))
			break;
		// strip the whitespace
		buf[--len] = '\0';
	}

	printf("read %d bytes '%s'\n", len, buf);
	return len;
}
