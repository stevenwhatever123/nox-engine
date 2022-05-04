#include <SaveLoadManager.h>

void NoxEngine::saveScene(std::string file_path, NoxEngine::GameState& game_state)
{
	std::ofstream outputStream(file_path, std::ios::binary);
	if (outputStream.is_open())
	{
		size_t numOfEntities = game_state.activeScene->entities.size();
		outputStream.write((char*)&numOfEntities, sizeof(size_t));

		for (Entity* ent : game_state.activeScene->entities)
		{
			// Getting the name
			std::string entityName(ent->name);
			size_t entityNameSize = entityName.size();
			outputStream.write((char*)&entityNameSize, sizeof(entityNameSize));
			outputStream.write((char*)entityName.c_str(), entityNameSize);

			// Get the file path if available
			bool loadedFromFile = ent->filepath != nullptr;
			outputStream.write((char*)&loadedFromFile, sizeof(bool));
			if (loadedFromFile)
			{
				std::string filepath(ent->filepath);
				size_t filepathSize = filepath.size();
				outputStream.write((char*)&filepathSize, sizeof(filepathSize));
				outputStream.write((char*)filepath.c_str(), filepathSize);
			}

			// Check if this entity have any component;
			bool hasComponent = ent->hasComp;
			outputStream.write((char*)&hasComponent, sizeof(bool));

			// If has component, write the component one by one
			if (hasComponent)
			{
				// Starting with position first
				bool hasTransform = ent->containsComps(TransformFlag);
				outputStream.write((char*)&hasTransform, sizeof(bool));

				if (hasTransform)
				{
					ITransform* transform = ent->getComp<TransformComponent>()->CastType<ITransform>();

					outputStream.write((char*)&transform->x, sizeof(f32));
					outputStream.write((char*)&transform->y, sizeof(f32));
					outputStream.write((char*)&transform->z, sizeof(f32));

					outputStream.write((char*)&transform->rx, sizeof(f32));
					outputStream.write((char*)&transform->ry, sizeof(f32));
					outputStream.write((char*)&transform->rz, sizeof(f32));

					outputStream.write((char*)&transform->sx, sizeof(f32));
					outputStream.write((char*)&transform->sy, sizeof(f32));
					outputStream.write((char*)&transform->sz, sizeof(f32));
				}

				// Renderable Component
				bool hasRenderableComp = ent->containsComps(RenderableFlag);
				outputStream.write((char*)&hasRenderableComp, sizeof(bool));

				if (hasRenderableComp)
				{
					if (loadedFromFile)
					{
						RenderableComponent* rendComp = ent->getComp<RenderableComponent>();

						// Pair using the mesh name associated with this entity
						Mesh2* mesh = (Mesh2*)rendComp;
						std::string meshName(mesh->name);
						size_t meshNameSize = meshName.size();
						outputStream.write((char*)&meshNameSize, sizeof(meshNameSize));
						outputStream.write((char*)meshName.c_str(), meshNameSize);

						std::string ambientTexture(rendComp->getAmbientTexture());
						size_t ambientTextureSize = ambientTexture.size();

						outputStream.write((char*)&ambientTextureSize, sizeof(ambientTextureSize));
						outputStream.write((char*)ambientTexture.c_str(), ambientTextureSize);
					}

				}
			}
		}

		outputStream.close();
	}
}

void NoxEngine::loadScene(std::string file_path, NoxEngine::GameState& game_state)
{
	std::ifstream inputStream(file_path, std::ios::binary);
	if (inputStream.is_open())
	{
		size_t numOfEntities;
		inputStream.read((char*)&numOfEntities, sizeof(size_t));

		for (u32 i = 0; i < numOfEntities; i++)
		{
			// Get the name
			std::string entityName;
			size_t entityNameSize;
			inputStream.read((char*)&entityNameSize, sizeof(entityNameSize));
			entityName.resize(entityNameSize);
			inputStream.read((char*)&entityName[0], entityNameSize);

			Entity* ent = nullptr;

			// Is it loaded from a file?
			bool loadedFromFile;
			inputStream.read((char*)&loadedFromFile, sizeof(bool));
			std::string fbx_filepath;
			if (loadedFromFile)
			{
				size_t fbx_filepathSize;
				inputStream.read((char*)&fbx_filepathSize, sizeof(fbx_filepathSize));
				fbx_filepath.resize(fbx_filepathSize);
				inputStream.read((char*)&fbx_filepath[0], fbx_filepathSize);

				ent = new Entity(game_state.activeScene, entityName.c_str(), fbx_filepath.c_str());
			}
			else
			{
				ent = new Entity(game_state.activeScene, entityName.c_str());
			}

			// Read component if available
			bool hasComponent;
			inputStream.read((char*)&hasComponent, sizeof(bool));

			if (hasComponent)
			{
				// Does it has a transform?
				bool hasTransform;
				inputStream.read((char*)&hasTransform, sizeof(bool));

				if (hasTransform)
				{
					TransformComponent* pos = new TransformComponent(0.0, 0.0, 0.0);

					inputStream.read((char*)&pos->x, sizeof(f32));
					inputStream.read((char*)&pos->y, sizeof(f32));
					inputStream.read((char*)&pos->z, sizeof(f32));

					inputStream.read((char*)&pos->rx, sizeof(f32));
					inputStream.read((char*)&pos->ry, sizeof(f32));
					inputStream.read((char*)&pos->rz, sizeof(f32));

					inputStream.read((char*)&pos->sx, sizeof(f32));
					inputStream.read((char*)&pos->sy, sizeof(f32));
					inputStream.read((char*)&pos->sz, sizeof(f32));

					ent->addComp(pos);
				}

				// Does it has renderable comp?
				bool hasRenderableComp;
				inputStream.read((char*)&hasRenderableComp, sizeof(bool));

				if (hasRenderableComp)
				{
					if (loadedFromFile)
					{
						game_state.meshScenes.emplace(fbx_filepath, NoxEngine::readFBX(fbx_filepath.c_str()));
						MeshScene& meshScene = game_state.meshScenes.find(fbx_filepath)->second;

						std::string meshName;
						size_t meshNameSize;
						inputStream.read((char*)&meshNameSize, sizeof(meshNameSize));
						meshName.resize(meshNameSize);
						inputStream.read((char*)&meshName[0], meshNameSize);

						RenderableComponent* rendComp = nullptr;

						for (Mesh2* mesh : meshScene.meshes)
						{
							if (strcmp(mesh->name.c_str(), meshName.c_str()) == 0)
							{
								rendComp = mesh;
								break;
							}
						}

						std::string ambientTexture;
						size_t ambientTextureSize;
						inputStream.read((char*)&ambientTextureSize, sizeof(ambientTextureSize));
						ambientTexture.resize(ambientTextureSize);
						inputStream.read((char*)&ambientTexture[0], ambientTextureSize);
						rendComp->ambientTexture = ambientTexture;

						ent->addComp(rendComp);
					}
				}
			}

			game_state.activeScene->addEntity(ent);
		}

		inputStream.close();
	}
}