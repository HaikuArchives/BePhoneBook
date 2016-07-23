#include <StorageKit.h>
#include <InterfaceKit.h>
#include <ClassInfo.h>

//Thanx to kmsf for this code

// charge une image archivée dans un fichier
BBitmap *load_flat_bitmap(const char *path)
{
	BBitmap *image;
	BArchivable *unarchived;

	BFile file(path,B_READ_ONLY);
	if (file.InitCheck()==B_NO_ERROR) 
	{
		BMessage flat;
		if (flat.Unflatten(&file)==B_OK) 
		{
			unarchived=instantiate_object(&flat);
			if (unarchived)
			{
				image=cast_as(unarchived,BBitmap);
				if (image) 
				{
					return image;
				}
			}
		}
	} 
	else
		return NULL;
}
