#include "scene.h"

#include <algorithm>
#include <memory>

namespace proto {

std::shared_ptr<Camera> Scene::get_camera()
{
	return camera;
}

void Scene::add_node(Node node)
{
	nodes.push_back(node);
}

void Scene::clear_nodes()
{
	nodes.clear();
}

} // namespace proto
