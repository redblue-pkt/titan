#ifndef LISTBOX_H
#define LISTBOX_H

int setlistboxselection(struct skin* listbox, char* childname)
{
	struct skin* child = NULL;
	int count = 0;

	if(listbox == NULL || childname == NULL)
	{
		err("NULL detect");
		return 1;
	}

	listbox->aktpage = -1;
	listbox->aktline = 1;

	child = listbox->next;
	while(child != NULL)
	{
		if(child->parentpointer == NULL)
		{
			if(child->parent == NULL)
			{
				child = child->next;
				continue;
			}
			else if(ostrcmp(child->parent, listbox->name) != 0 || child->hidden == YES || child->deaktivcol > -1)
			{
				child = child->next;
				continue;
			}
		}
		else if(child->parentpointer != listbox || child->hidden == YES || child->deaktivcol > -1)
		{
			child = child->next;
			continue;
		}

		count++;
		if(ostrcmp(child->name, childname) == 0)
		{
			listbox->aktpage = -1;
			listbox->aktline = count;
			break;
		}

		child = child->next;
	}

	return 0;
}

struct skin* addlistbox(struct skin* screen, struct skin* listbox, struct skin* last, int del)
{
	struct skin* node = NULL;

	node = addscreennode(screen, NULL, last);
	if(node != NULL)
	{
		node->bordercol = listbox->bordercol;
		node->width = 100;
		node->prozwidth = 1;
		node->height = listbox->fontsize + 2;
		node->parentpointer = listbox;
		node->del = del;
		node->deaktivcol = -1;
	}

	return node;
}

#endif
