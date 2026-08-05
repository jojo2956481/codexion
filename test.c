if (data->scheduler[0] == 'l')
{
	for (int i = count; i > 0; i--)
		data->queue.coder_ids[i] = data->queue.coder_ids[i - 1];
	data->queue.coder_ids[0] = c->id;
}