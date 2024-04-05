#include "global_scene.h"

#include "../rendering/scene.h"

namespace proto::scene {

static Scene global_scene;
	
Scene& get_scene()
{
	return global_scene;
}

}
