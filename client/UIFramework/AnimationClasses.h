#pragma once

#include "ImageClassesFwd.h"
#include "AnimationClassesFwd.h"

/*
 * AnimationClasses.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

/*
 * Interface for VCMI related animation tasks like player coloring
 */
class IAnimationTasks : public IGraphicsTasks
{
public:
	// Change palette to specific player.
	virtual TAnimationPtr recolorToPlayer(int player) const =0;

	// Sets/Unsets the yellow or blue glow animation effect.
	virtual TAnimationPtr setGlowAnimation(EGlowAnimationType::EGlowAnimationType glowType, ui8 alpha) const =0;

	virtual TAnimationPtr rotate(EImageRotation::EImageRotation rotation) const =0;
};

class IAnimation : public IAnimationTasks
{
protected:
	std::map<size_t, size_t> entries;
	
	// Stores the filesystem location, loaded groups and transformations of the animation.
	GraphicsLocator locator;

public:
	IAnimation(const GraphicsLocator & Locator = GraphicsLocator());

	virtual ~IAnimation() { };
	virtual IAnimation * clone() const =0;
	
	// Gets the locator object which stores the filesystem location, 
	// loaded groups and transformations of the animation.
	const GraphicsLocator & getLocator() const;
	
	// Gets the information how many groups/frames exist
	std::map<size_t, size_t> getEntries() const;

	// Gets the index of the loaded group. -1 if all groups are loaded.
	si8 getLoadedGroup() const;

	virtual void draw(TImagePtr where, size_t frame, size_t group, int posX, int posY) const =0;
};

class CImageBasedAnimation : public IAnimation
{
	// images[group][frame], store objects with loaded images
	std::map<size_t, std::map<size_t, TMutableImagePtr> > images;

public:
	// Loads frames of the specified group of an animation. Assign -1 to the second parameter 'group' to load all groups.
	CImageBasedAnimation(const CDefFile * defFile, size_t group = -1, const GraphicsLocator & Locator = GraphicsLocator());
	
	CImageBasedAnimation(const CImageBasedAnimation & other);
	CImageBasedAnimation & operator=(const CImageBasedAnimation & other); 

	IAnimation * clone() const;

	void draw(TImagePtr where, size_t frame, size_t group, int posX, int posY) const;

	TAnimationPtr recolorToPlayer(int player) const;
	void recolorToPlayer(int player);
	void recolorToPlayerViaSelector(const GraphicsSelector & selector);

	TAnimationPtr setGlowAnimation(EGlowAnimationType::EGlowAnimationType glowType, ui8 alpha) const;
	void setGlowAnimation(EGlowAnimationType::EGlowAnimationType glowType, ui8 alpha) { };

	TAnimationPtr rotate(EImageRotation::EImageRotation rotation) const;
	void rotate(EImageRotation::EImageRotation rotation) { };
};

class CAnimation
{
	TAnimationPtr anim;
	size_t currentGroup, currentFrame, frameCount;
	double currentTime;
	bool repeat;

public:
	CAnimation(TAnimationPtr animation);
	CAnimation(const ResourceIdentifier & identifier);
	CAnimation(const ResourceIdentifier & identifier, size_t group, bool repeat = false);

	void setGroup(size_t group, bool repeat = false);

	void update(double elapsedTime);
	void draw(TImagePtr where, int posX, int posY);

	void recolorToPlayer(int player);
	void setGlowAnimation(EGlowAnimationType::EGlowAnimationType glowType, ui8 alpha);
	void rotate(EImageRotation::EImageRotation rotation);
};