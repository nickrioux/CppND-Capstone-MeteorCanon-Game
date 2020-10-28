#include "spritecomponent.h"

SpriteComponent::SpriteComponent(const string & filePath, bool isFixed) : _transform(nullptr), _isFixed(isFixed), _animationSpeed(0), _isAnimated(false), _numFrames(0) {
    SetTexture(filePath);
}

SpriteComponent::SpriteComponent(const std::string & id, int numFrames, int animationSpeed, bool isFixed) : _isAnimated(true), _numFrames(numFrames), _animationSpeed(animationSpeed), _isFixed(isFixed) {
    SetTexture(id);
}

void SpriteComponent::SetTexture(const std::string & assetTextureId) {
    _texture = Game::GetGame().GetAssetManager().GetTexture(assetTextureId);
}

void SpriteComponent::Init() {
    _transform = GetOwner()->GetComponent<TransformComponent>();
    _sourceRectangle.x = 0;
    _sourceRectangle.y = 0;
    _sourceRectangle.w = _transform->GetWidth();
    _sourceRectangle.h = _transform->GetHeight();
}

void SpriteComponent::Update(float deltaTime)  {

    if (_isAnimated) {
        _sourceRectangle.x = _sourceRectangle.w * static_cast<int>((SDL_GetTicks() / _animationSpeed) % _numFrames);
    }

    _destinationRectangle.x = static_cast<int>(_transform->GetPosition().x) - (_isFixed ? 0 : Game::GetGame().GetCamera().x);
    _destinationRectangle.y = static_cast<int>(_transform->GetPosition().y) - (_isFixed ? 0 : Game::GetGame().GetCamera().y);

    _destinationRectangle.w = _transform->GetWidth() * _transform->GetScale();
    _destinationRectangle.h = _transform->GetHeight() * _transform->GetScale();
}

void SpriteComponent::Render() {
    TextureManager::Draw(_texture, _sourceRectangle, _destinationRectangle, _spriteFlip);
}