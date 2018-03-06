#pragma once

#include "SDK.h"
#include "Global.h"

struct Hitbox
{
	Hitbox(void)
	{
		hitbox = -1;
	}

	Hitbox(int newHitBox)
	{
		hitbox = newHitBox;
	}

	int  hitbox;
	Vector points[9];
};

class HitboxMgr
{
public:
	static bool GetHitbox(CBaseEntity* target, Hitbox* hitbox, float pointScale, matrix3x4_t *customMatrix = NULL)
	{
		int id = target->GetIndex();

		//if (!G::boneCache[id].valid)
		//	return false;

		if (!target->GetModel())
			return false;

		studiohdr_t *hdr = I::pModelInfo->GetStudioModel((model_t*)target->GetModel());

		if (!hdr)
			return false;

		mstudiohitboxset_t *hitboxSet = hdr->pHitboxSet(target->GetHitboxSet());

		if (!hitboxSet)
			return false;

		mstudiobbox_t *untransformedBox = hitboxSet->pHitbox(hitbox->hitbox);
		
		if (!untransformedBox)
			return false;

		Vector bbmin = untransformedBox->bbmin;
		Vector bbmax = untransformedBox->bbmax;

		if (untransformedBox->radius != -1.f)
		{
			bbmin -= Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
			bbmax += Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
		}

		// point scale

		bbmin *= pointScale;
		bbmax *= pointScale;

		Vector points[] = { ((bbmin + bbmax) * .5f),
			Vector(bbmin.x, bbmin.y, bbmin.z),
			Vector(bbmin.x, bbmax.y, bbmin.z),
			Vector(bbmax.x, bbmax.y, bbmin.z),
			Vector(bbmax.x, bbmin.y, bbmin.z),
			Vector(bbmax.x, bbmax.y, bbmax.z),
			Vector(bbmin.x, bbmax.y, bbmax.z),
			Vector(bbmin.x, bbmin.y, bbmax.z),
			Vector(bbmax.x, bbmin.y, bbmax.z) };

		matrix3x4_t *matrix = (customMatrix == NULL && G::boneCache[id].valid ? G::boneCache[id].pBoneToWorldOut : customMatrix);

		if (!matrix)
			return false;

		for (int index = 0; index <= 8; ++index)
		{
			if (index != 0)
				points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);

			Math::VectorTransform(points[index], matrix[untransformedBox->bone], hitbox->points[index]);
		}

		return true;
	}

	static void FixPosition(CBaseEntity* target, Hitbox& hitbox)
	{
		Vector center = hitbox.points[0];

		if (hitbox.hitbox == HITBOX_HEAD)
		{
			/*Vector high = ((hitbox.points[3] + hitbox.points[5]) * .5f);

			float pitch = target->GetEyeAngles().x;

			if ((pitch > 0.f) && (pitch <= 89.f))
			{
				Vector height = (((high - hitbox.points[0]) / 3) * 4);
				Vector new_height = (hitbox.points[0] + (height * (pitch / 89.f)));

				hitbox.points[0] = new_height;
			}
			else if ((pitch < 292.5f) && (pitch >= 271.f))
			{
				hitbox.points[0] -= Vector(0.f, 0.f, 1.f);
			}*/

			//if cfg accuracy boost
			//hitbox.points[0] += Vector(0.0f, 0.0f, 4.5f);
		}	
	}
};
