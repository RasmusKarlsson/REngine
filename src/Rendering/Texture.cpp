///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include <lodepng.cpp>
#include "hdrloader.h"
#include "Texture.h"
#include "Stats.h"

#include <iostream>

#include "RenderingAPI/RendererContext.h"

std::vector<Texture*> Texture::m_textures;

Texture::Texture()
{

	m_textures.push_back(this);
}

Texture::~Texture()
{
	auto it = m_textures.begin();
	while (it != m_textures.end())
	{
		auto curr = it++;
		if (*curr == this) {
			m_textures.erase(curr);
		}
	}
}

Texture::Texture(string filename)
{
	CreateFromPNG(filename);
}


void Texture::CreateFromPNG(string filename)
{
	//If it already exists, remove it
	if(m_textureID)
	{
		RendererContext::DeleteTexture(m_textureID);
	}
	
	m_fileName = filename;

	m_textureFileFormat = RENGINE::TEXTURE_FILE_FORMAT_PNG;

	unsigned int error = lodepng::decode(m_imageData, m_textureWidth, m_textureHeight, m_fileName.c_str());

	if (error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << " at file path: " << filename << std::endl;
		return;
	}

	RENGINE::PIXEL_FORMAT pixelFormat = RENGINE::PIXEL_FORMAT_RGB;
		
	if (m_imageData.size() == m_textureWidth* m_textureHeight*4) pixelFormat = RENGINE::PIXEL_FORMAT_RGBA;
	
	m_textureID = RendererContext::CreateTexture(m_textureWidth, m_textureHeight, pixelFormat, RENGINE::PIXEL_FORMAT_RGBA, RENGINE::PIXEL_TYPE_UNSIGNED_BYTE, &m_imageData[0]);

	if(!m_keepImageData)
		m_imageData.clear();
	
	RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE_TEXTURE_2D, RENGINE::TEXTURE_PNAME_TEXTURE_MAG_FILTER, RENGINE::TEXTURE_PARAM_LINEAR);
	RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE_TEXTURE_2D, RENGINE::TEXTURE_PNAME_TEXTURE_MIN_FILTER, RENGINE::TEXTURE_PARAM_LINEAR_MIPMAP_NEAREST);
	RendererContext::GenerateMipmaps();
}

void Texture::CreateFromHDR(string filename)
{
	//If it already exists, remove it
	if (m_textureID)
	{
		RendererContext::DeleteTexture(m_textureID);
	}
	
	m_fileName = filename;
	HDRLoaderResult hdrTexture;
	bool result = HDRLoader::load(m_fileName.c_str(), hdrTexture);
	//TODO: Deal with result
	m_textureWidth = hdrTexture.width;
	m_textureHeight = hdrTexture.height;
	m_textureID = RendererContext::CreateTexture(m_textureWidth, m_textureHeight, RENGINE::PIXEL_FORMAT_RGBA32F, RENGINE::PIXEL_FORMAT_RGB, RENGINE::PIXEL_TYPE_FLOAT, (uint8*)hdrTexture.cols);

	if (!m_keepImageData)
		m_imageData.clear();
	
	m_textureFileFormat = RENGINE::TEXTURE_FILE_FORMAT_HDR;

	RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE_TEXTURE_2D, RENGINE::TEXTURE_PNAME_TEXTURE_MAG_FILTER, RENGINE::TEXTURE_PARAM_LINEAR);
	RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE_TEXTURE_2D, RENGINE::TEXTURE_PNAME_TEXTURE_MIN_FILTER, RENGINE::TEXTURE_PARAM_LINEAR);

	RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE_TEXTURE_2D, RENGINE::TEXTURE_PNAME_TEXTURE_WRAP_T, RENGINE::TEXTURE_PARAM_CLAMP);
	RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE_TEXTURE_2D, RENGINE::TEXTURE_PNAME_TEXTURE_WRAP_R, RENGINE::TEXTURE_PARAM_CLAMP);

}

uint32 Texture::GetTextureID()
{
	if (m_dirty)
	{
		if (m_textureFileFormat == RENGINE::TEXTURE_FILE_FORMAT_PNG)
			CreateFromPNG(m_fileName);
		else
			CreateFromHDR(m_fileName);
		m_dirty = false;
	}
	
	return m_textureID;
};

void Texture::Bind(uint32 slot)
{
	if (m_dirty)
	{
		if(m_textureFileFormat == RENGINE::TEXTURE_FILE_FORMAT_PNG)
			CreateFromPNG(m_fileName);
		else
			CreateFromHDR(m_fileName);

		m_dirty = false;
	}
	if (s_boundTextures[slot] != m_textureID)
	{
		RendererContext::BindTexture(m_textureID, slot);
		s_boundTextures[slot] = m_textureID;
		Stats::s_textureBounds++;
	}
}

bool Texture::IsImageFileExtension(std::string texturePath)
{
	if (texturePath.find(".png") != std::string::npos) return true;
	if (texturePath.find(".PNG") != std::string::npos) return true;
	if (texturePath.find(".hdr") != std::string::npos) return true;
	if (texturePath.find(".HDR") != std::string::npos) return true;
	return false;
}

void Texture::SetTextureDirty(std::string texturePath)
{
	for each(Texture* texture in m_textures)
	{
		if (texture->m_fileName == texturePath)
		{
			texture->SetDirty();
		}
	}
}