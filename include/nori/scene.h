/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2012 by Wenzel Jakob and Steve Marschner.

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(__SCENE_H)
#define __SCENE_H

#include <nori/kdtree.h>

NORI_NAMESPACE_BEGIN

/**
 * \brief Main scene data structure
 *
 * This class holds information on scene objects and is responsible for
 * coordinating rendering jobs. It also provides useful query routines that 
 * are mostly used by the \ref Integrator implementations.
 */
class Scene : public NoriObject {
public:
	/// Construct a new scene object
	Scene(const PropertyList &);
	
	/// Release all memory
	virtual ~Scene();

	/// Return a pointer to the scene's kd-tree
	inline const KDTree *getKDTree() const { return m_kdtree; }

	/// Return a pointer to the scene's integrator
	inline const Integrator *getIntegrator() const { return m_integrator; }
	
	/// Return a pointer to the scene's camera
	inline const Camera *getCamera() const { return m_camera; }
	
	/// Return a pointer to the scene's sample generator (const version)
	inline const Sampler *getSampler() const { return m_sampler; }
	
	/// Return a pointer to the scene's sample generator
	inline Sampler *getSampler() { return m_sampler; }

	/// Return a pointer to the scene's medium (if any)
	inline const Medium *getMedium() const { return m_medium; }

	/// Return a reference to an array containing all meshes
	inline const std::vector<Mesh *> &getMeshes() const { return m_meshes; }

	/**
	 * \brief Intersect a ray against all triangles stored in the scene
	 * and return detailed intersection information
	 *
	 * \param ray
	 *    A 3-dimensional ray data structure with minimum/maximum
	 *    extent information
	 *
	 * \param its
	 *    A detailed intersection record, which will be filled by the
	 *    intersection query
	 *
	 * \return \c true if an intersection was found
	 */
	inline bool rayIntersect(const Ray3f &ray, Intersection &its) const {
		return m_kdtree->rayIntersect(ray, its, false);
	}

	/**
	 * \brief Intersect a ray against all triangles stored in the scene
	 * and \a only determine whether or not there is an intersection.
	 * 
	 * This method much faster than the other ray tracing function,
	 * but the performance comes at the cost of not providing any
	 * additional information about the detected intersection 
	 * (not even its position).
	 *
	 * \param ray
	 *    A 3-dimensional ray data structure with minimum/maximum
	 *    extent information
	 *
	 * \return \c true if an intersection was found
	 */
	inline bool rayIntersect(const Ray3f &ray) const {
		Intersection its; /* Unused */
		return m_kdtree->rayIntersect(ray, its, true);
	}

	/**
	 * \brief Return an axis-aligned box that bounds the scene
	 */
	inline const BoundingBox3f &getBoundingBox() const {
		return m_kdtree->getBoundingBox();
	}

	/**
	 * \brief Inherited from \ref NoriObject::activate()
	 *
	 * Initializes the internal data structures (kd-tree,
	 * luminaire sampling data structures, etc.)
	 */
	void activate();

	/// Add a child object to the scene (meshes, integrators etc.)
	void addChild(NoriObject *obj);

	/// Return a brief string summary of the instance (for debugging purposes)
	QString toString() const;

	EClassType getClassType() const { return EScene; }
private:
	std::vector<Mesh *> m_meshes;
	Integrator *m_integrator;
	Sampler *m_sampler;
	Camera *m_camera;
	Medium *m_medium;
	KDTree *m_kdtree;
};

NORI_NAMESPACE_END

#endif /* __SCENE_H */
