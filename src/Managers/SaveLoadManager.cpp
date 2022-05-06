#include <Managers/SaveLoadManager.h>

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
						Mesh* mesh = (Mesh*)rendComp;
						std::string meshName(mesh->name);
						size_t meshNameSize = meshName.size();
						outputStream.write((char*)&meshNameSize, sizeof(meshNameSize));
						outputStream.write((char*)meshName.c_str(), meshNameSize);

						for (RendObj obj : game_state.renderer->getObjects())
						{
							if (obj.ent == ent)
							{
								std::string ambientTexture(obj.ambientTexturePath);
								size_t ambientTextureSize = ambientTexture.size();

								outputStream.write((char*)&ambientTextureSize, sizeof(ambientTextureSize));
								outputStream.write((char*)ambientTexture.c_str(), ambientTextureSize);

								std::string normalTexture(obj.normalTexturePath);
								size_t normalTextureSize = normalTexture.size();

								outputStream.write((char*)&normalTextureSize, sizeof(normalTextureSize));
								outputStream.write((char*)normalTexture.c_str(), normalTextureSize);

								break;
							}
						}
					}

				}

				// The assumption is if there's a renderable component
				// It may also has an animation component
				// Animation Component
				bool hasAnimationComp = ent->containsComps(AnimationFlag);
				outputStream.write((char*)&hasAnimationComp, sizeof(bool));

				if (hasAnimationComp)
				{
					AnimationComponent* animComp = ent->getComp<AnimationComponent>();

					size_t numAnimation = animComp->numTicks.size();
					outputStream.write((char*)&numAnimation, sizeof(numAnimation));
					// numTicks
					outputStream.write((char*)&animComp->numTicks[0], numAnimation * sizeof(i32));

					// Animation duration
					outputStream.write((char*)&animComp->animationDuration[0], numAnimation * sizeof(time_type));

					// maximumFrame
					outputStream.write((char*)&animComp->maximumFrame[0], numAnimation * sizeof(u32));

					
					for (u32 i = 0; i < numAnimation; i++)
					{
						size_t numKeyFrame = animComp->transformation[i].size();
						outputStream.write((char*)&numKeyFrame, sizeof(numKeyFrame));

						for (u32 j = 0; j < numKeyFrame; j++)
						{
							// nodeAnimTransformation
							outputStream.write((char*)&animComp->transformation[i][j], sizeof(mat4));

							// nodeAnimTranslationMatrices
							outputStream.write((char*)&animComp->translationMatrices[i][j], sizeof(mat4));

							// eulerAngleXYZ
							outputStream.write((char*)&animComp->eulerAngleXYZ[i][j], sizeof(glm::vec3));

							// nodeAnimRotationMatrices
							outputStream.write((char*)&animComp->rotationMatrices[i][j], sizeof(mat4));

							// nodeAnimScalingMatrices
							outputStream.write((char*)&animComp->scalingMatrices[i][j], sizeof(mat4));
						}
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
						if(game_state.meshScenes.find(fbx_filepath) == game_state.meshScenes.end())
							game_state.meshScenes.emplace(fbx_filepath, NoxEngine::readFBX(fbx_filepath.c_str()));
						MeshScene& meshScene = game_state.meshScenes.find(fbx_filepath)->second;

						std::string meshName;
						size_t meshNameSize;
						inputStream.read((char*)&meshNameSize, sizeof(meshNameSize));
						meshName.resize(meshNameSize);
						inputStream.read((char*)&meshName[0], meshNameSize);

						RenderableComponent* rendComp = nullptr;

						for (Mesh* mesh : meshScene.meshes)
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

						std::string normalTexture;
						size_t normalTextureSize;
						inputStream.read((char*)&normalTextureSize, sizeof(normalTextureSize));
						normalTexture.resize(normalTextureSize);
						inputStream.read((char*)normalTexture[0], normalTextureSize);
						rendComp->normalTexture = normalTexture;

						ent->addComp(rendComp);
					}
				}

				// The assumption is if there's a renderable component
				// It may also has an animation component
				// Animation Component
				bool hasAnimationComp;
				inputStream.read((char*)&hasAnimationComp, sizeof(bool));

				if (hasAnimationComp)
				{

					RenderableComponent* rendComp = ent->getComp<RenderableComponent>();
					MeshNode* meshnode = nullptr;

					MeshScene& meshScene = game_state.meshScenes.find(fbx_filepath)->second;

					// Get the node associated to this mesh
					for (MeshNode* node : meshScene.allNodes)
					{
						if (node->hasAnimations() && meshScene.meshes[node->meshIndex[0]] == rendComp)
						{
							meshnode = node;
							break;
						}
					}

					AnimationComponent* animComp = new AnimationComponent(meshScene, meshnode);

					size_t numAnimation;
					inputStream.read((char*)&numAnimation, sizeof(numAnimation));

					animComp->numTicks.resize(numAnimation);
					animComp->animationDuration.resize(numAnimation);

					animComp->transformation.resize(numAnimation);
					animComp->translationMatrices.resize(numAnimation);
					animComp->rotationMatrices.resize(numAnimation);
					animComp->scalingMatrices.resize(numAnimation);
					animComp->maximumFrame.resize(numAnimation);
					
					// numTicks
					inputStream.read((char*)&animComp->numTicks[0], numAnimation * sizeof(i32));

					// Animation duration
					inputStream.read((char*)&animComp->animationDuration[0], numAnimation * sizeof(time_type));

					// maximumFrame
					inputStream.read((char*)&animComp->maximumFrame[0], numAnimation * sizeof(u32));

					for (u32 i = 0; i < numAnimation; i++)
					{
						size_t numKeyFrame;
						inputStream.read((char*)&numKeyFrame, sizeof(numKeyFrame));

						animComp->transformation[i].resize(numKeyFrame);
						animComp->translationMatrices[i].resize(numKeyFrame);
						animComp->eulerAngleXYZ[i].resize(numKeyFrame);
						animComp->rotationMatrices[i].resize(numKeyFrame);
						animComp->scalingMatrices[i].resize(numKeyFrame);

						for (u32 j = 0; j < numKeyFrame; j++)
						{
							// nodeAnimTransformation
							inputStream.read((char*)&animComp->transformation[i][j], sizeof(mat4));

							// nodeAnimTranslationMatrices
							inputStream.read((char*)&animComp->translationMatrices[i][j], sizeof(mat4));

							// eulerAngleXYZ
							inputStream.read((char*)&animComp->eulerAngleXYZ[i][j], sizeof(glm::vec3));

							// nodeAnimRotationMatrices
							inputStream.read((char*)&animComp->rotationMatrices[i][j], sizeof(mat4));

							// nodeAnimScalingMatrices
							inputStream.read((char*)&animComp->scalingMatrices[i][j], sizeof(mat4));
						}
					}

					ent->addComp(animComp);
				}
			}

			game_state.activeScene->addEntity(ent);
		}

		inputStream.close();
	}
}