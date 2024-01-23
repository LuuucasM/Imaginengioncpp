#include "impch.h"
#include "Subtexture2D.h"

namespace IM {
	SubTexture2D::SubTexture2D(const RefPtr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) 
		: _Texture(texture) {
		_TexCoords[0] = {min.x, min.y};
		_TexCoords[1] = { max.x, min.y };
		_TexCoords[2] = { max.x, max.y };
		_TexCoords[3] = { min.x, max.y };
	}
	RefPtr<SubTexture2D> SubTexture2D::CreateFromCoords(const RefPtr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		return CreateRefPtr<SubTexture2D>(texture, min, max);
	}
}