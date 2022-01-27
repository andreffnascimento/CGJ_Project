#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"
#include "engine/text/avtFreeType.h"

#include "engine/scene/components/lensFlareComponent.h"


inline int clampi(const int x, const int min, const int max) {
	return (x < min ? min : (x > max ? max : x));
}

void Renderer::renderLensFlare(const Scene& scene) const
{
	const auto& lensFlareComponents = scene.getSceneComponents<LensFlareComponent>();

	for (const auto& lensFlareIterator : lensFlareComponents) {
		const LensFlareComponent lensFlare = lensFlareIterator.second;


		int flarePos[2];
		int m_viewport[4];

		//CURRENT SUN POSITION. HOW TO ABSTRACT? FIXME
		float lightPos[4] = { 299.0f, 99.0f, 169.0f, 1.0f };
		float lightScreenPos[4];


		glGetIntegerv(GL_VIEWPORT, m_viewport);

		pushMatrix(MODEL);
		loadIdentity(MODEL);
		computeDerivedMatrix(PROJ_VIEW_MODEL);

		if (!project(lightPos, lightScreenPos, m_viewport))
			printf("Error in getting projected light in screen\n");
		flarePos[0] = clampi((int)lightScreenPos[0], m_viewport[0], m_viewport[0] + m_viewport[2] - 1);
		flarePos[1] = clampi((int)lightScreenPos[1], m_viewport[1], m_viewport[1] + m_viewport[3] - 1);

		_initLensFlareRendering();

		_renderFlare(lensFlare.flareData(), flarePos[0], flarePos[0], m_viewport);

		_terminateLensFlareRendering();
	}

}

void Renderer::_initLensFlareRendering() const
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Renderer::_renderFlare(LensFlareComponent::FlareData flare, int locX, int locY, int* m_viewport) const
{
	/*int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int		cx, cy;
	float    maxflaredist, flaredist, flaremaxsize, flarescale, scaleDistance;
	int     width, height, alpha;    // Piece parameters;
	int     i;
	float	diffuse[4];

	GLint loc;
	GLint texMode_uniformId, tex_loc;

	int screenMaxCoordX = m_viewport[0] + m_viewport[2] - 1;
	int screenMaxCoordY = m_viewport[1] + m_viewport[3] - 1;

	//viewport center
	cx = m_viewport[0] + (int)(0.5f * (float)m_viewport[2]) - 1;
	cy = m_viewport[1] + (int)(0.5f * (float)m_viewport[3]) - 1;

	// Compute how far off-center the flare source is.
	maxflaredist = sqrt(cx * cx + cy * cy);
	flaredist = sqrt((locX - cx) * (locX - cx) + (locY - cy) * (locY - cy));
	scaleDistance = (maxflaredist - flaredist) / maxflaredist;
	flaremaxsize = (int)(m_viewport[2] * flare.fMaxSize);
	flarescale = (int)(m_viewport[2] * flare.fScale);

	// Destination is opposite side of centre from source
	dx = clampi(cx + (cx - locX), m_viewport[0], screenMaxCoordX);
	dy = clampi(cy + (cy - locY), m_viewport[1], screenMaxCoordY);

	// Render each element. To be used Texture Unit 0

	//FROM SETUP SHADERS - FIXME
	//glUniform1i(texMode_uniformId, 3); // draw modulated textured particles
	//glUniform1i(tex_loc, 0);  //use TU 0

	for (i = 0; i < flare.nPieces; ++i)
	{
		// Position is interpolated along line between start and destination.
		px = (int)((1.0f - flare.element[i].fDistance) * locX + flare.element[i].fDistance * dx);
		py = (int)((1.0f - flare.element[i].fDistance) * locY + flare.element[i].fDistance * dy);
		px = clampi(px, m_viewport[0], screenMaxCoordX);
		py = clampi(py, m_viewport[1], screenMaxCoordY);

		// Piece size are 0 to 1; flare size is proportion of screen width; scale by flaredist/maxflaredist.
		width = (int)(scaleDistance * flarescale * flare.element[i].fSize);

		// Width gets clamped, to allows the off-axis flaresto keep a good size without letting the elements get big when centered.
		if (width > flaremaxsize)  width = flaremaxsize;

		height = (int)((float)m_viewport[3] / (float)m_viewport[2] * (float)width);
		memcpy(diffuse, flare.element[i].matDiffuse, 4 * sizeof(float));
		diffuse[3] *= scaleDistance;   //scale the alpha channel


		//DON'T KNOW HOW TO ACCESS SHADERS IN RENDERER FIXME
		/*if (width > 1)
		{
			// send the material - diffuse color modulated with texture
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, diffuse);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FlareTextureArray[flare.element[i].textureId]);
			pushMatrix(MODEL);
			translate(MODEL, (float)(px - width * 0.0f), (float)(py - height * 0.0f), 0.0f);
			scale(MODEL, (float)width, (float)height, 1);
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			glBindVertexArray(myMeshes[6].vao);
			glDrawElements(myMeshes[6].type, myMeshes[6].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			popMatrix(MODEL);
		}
	}*/

}

void Renderer::_terminateLensFlareRendering() const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}