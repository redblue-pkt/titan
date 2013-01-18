#ifndef MARKER_H
#define MARKER_H

struct marker* addmarkernode(off64_t pos)
{
	
	struct marker *newnode = NULL;
	struct marker *oldnode = NULL;
	struct marker *prev = NULL;
	
	newnode = (struct marker*)calloc(1, sizeof(struct marker));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}
	newnode->next = NULL;
	newnode->pos = pos;
	
	if(status.playmarker == NULL)
	{
		newnode->prev = NULL;
		status.playmarker = newnode;
		return newnode;
	}
	
	oldnode = status.playmarker;
	if(pos == -1)
	{
		while(oldnode->next != NULL)
		{
			oldnode = oldnode->next;
		}
		oldnode->next = newnode;
		newnode->prev = oldnode;
	}
	else
	{
		prev = oldnode;
		while(oldnode->next != NULL && oldnode->pos < pos)
		{
			prev = oldnode;
			oldnode = oldnode->next;
		}
		
		if(oldnode->prev == NULL)
		{
			if(oldnode->pos > pos)
			{
				newnode->next = oldnode;
				newnode->prev = NULL;
				oldnode->prev = newnode;
				status.playmarker = newnode;
				return newnode;
			}
		}
		if(oldnode->next == NULL)
		{
			if(oldnode->pos < pos)
			{
				newnode->next = NULL;
				newnode->prev = oldnode;
				oldnode->next = newnode;
				return newnode;
			}
		}
		newnode->next = prev->next;
		newnode->prev = prev;
		if(prev->next != NULL)
				prev->next->prev = newnode;
		prev->next = newnode;
	}
	
	return newnode;
}


int delmarkernode(off64_t pos)
{
	if(status.playmarker == NULL)
		return -1;
	
	struct marker* delnode;
	struct marker* node = status.playmarker;
	
	delnode = node;
	while(delnode != NULL)
	{
		printf("--> %i",delnode->pos);
		node = delnode->next;
		if(pos == -1)
		{
			free(delnode->time);
			free(delnode);
			status.playmarker == NULL;
		}
		else if(delnode->pos == pos)
		{
			if(delnode->next != NULL)
				delnode->next->prev = delnode->prev;
			if(delnode->prev == NULL)
				status.playmarker = delnode->next;
			else
				delnode->prev->next = delnode->next;
			free(delnode->time);
			free(delnode);
			break;
		}
		delnode = node;
	}
	return 0;
}
