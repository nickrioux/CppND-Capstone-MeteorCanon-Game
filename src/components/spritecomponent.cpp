#include "entity.h"

#include "spritecomponent.h"

SpriteComponent::SpriteComponent(const std::string & textureId, bool isFixed) 
                : isFixed_{isFixed}, texture_{Game::GetGame().GetAssetManager().GetTexture(textureId)} {
}

SpriteComponent::SpriteComponent(const std::string & textureId, int numFrames, int animationSpeed, bool isFixed) 
                : isFixed_{isFixed}, isAnimated_{true}, 
                  numFrames_{numFrames}, animationSpeed_{animationSpeed},
                  texture_{Game::GetGame().GetAssetManager().GetTexture(textureId)} {
}

void SpriteComponent::SetTexture(const std::string & assetTextureId) {
    texture_ = Game::GetGame().GetAssetManager().GetTexture(assetTextureId);
}

void SpriteComponent::Init() {
    transform_ = GetOwner()->GetComponent<TransformComponent>();
    sourceRectangle_.x = 0;
    sourceRectangle_.y = 0;
    sourceRectangle_.w = transform_->GetWidth();
    sourceRectangle_.h = transform_->GetHeight();
    destinationRectangle_.x = static_cast<int>(transform_->GetPosition().x) - (isFixed_ ? 0 : Game::GetGame().GetCamera().x);
    destinationRectangle_.y = static_cast<int>(transform_->GetPosition().y) - (isFixed_ ? 0 : Game::GetGame().GetCamera().y);
    destinationRectangle_.w = transform_->GetWidth() * transform_->GetScale();
    destinationRectangle_.h = transform_->GetHeight() * transform_->GetScale();
}

void SpriteComponent::Update(float deltaTime)  {

    if (isAnimated_) {
        sourceRectangle_.x = sourceRectangle_.w * static_cast<int>((SDL_GetTicks() / animationSpeed_) % numFrames_);
    }

    destinationRectangle_.x = static_cast<int>(transform_->GetPosition().x) - (isFixed_ ? 0 : Game::GetGame().GetCamera().x);
    destinationRectangle_.y = static_cast<int>(transform_->GetPosition().y) - (isFixed_ ? 0 : Game::GetGame().GetCamera().y);

    destinationRectangle_.w = transform_->GetWidth() * transform_->GetScale();
    destinationRectangle_.h = transform_->GetHeight() * transform_->GetScale();
}

void SpriteComponent::Render() {
    TextureManager::Draw(texture_, sourceRectangle_, destinationRectangle_, spriteFlip_);
}