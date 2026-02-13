#include "CTile.h"

CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::Save(FILE* File)
{
	fwrite(&Type, sizeof(ETileType), 1, File);
	fwrite(&Pos, sizeof(FVector2), 1, File);
	fwrite(&Size, sizeof(FVector2), 1, File);
	fwrite(&Center, sizeof(FVector2), 1, File);
	fwrite(&TextureFrame, sizeof(int), 1, File);
	fwrite(&FrameStart, sizeof(FVector2), 1, File);
	fwrite(&FrameEnd, sizeof(FVector2), 1, File);

	fwrite(&bRender, sizeof(bool), 1, File);
	fwrite(&bOutlineRender, sizeof(bool), 1, File);

	fwrite(&OutlineColor, sizeof(FVector4), 1, File);

	fwrite(&IndexX, sizeof(int), 1, File);
	fwrite(&IndexY, sizeof(int), 1, File);
	fwrite(&Index, sizeof(int), 1, File);
}

void CTile::Load(FILE* File)
{
	fread(&Type, sizeof(ETileType), 1, File);
	fread(&Pos, sizeof(FVector2), 1, File);
	fread(&Size, sizeof(FVector2), 1, File);
	fread(&Center, sizeof(FVector2), 1, File);
	fread(&TextureFrame, sizeof(int), 1, File);
	fread(&FrameStart, sizeof(FVector2), 1, File);
	fread(&FrameEnd, sizeof(FVector2), 1, File);

	fread(&bRender, sizeof(bool), 1, File);
	fread(&bOutlineRender, sizeof(bool), 1, File);

	fread(&OutlineColor, sizeof(FVector4), 1, File);

	fread(&IndexX, sizeof(int), 1, File);
	fread(&IndexY, sizeof(int), 1, File);
	fread(&Index, sizeof(int), 1, File);
}
