/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkEulerTransform_h
#define itkEulerTransform_h

#include "itkAdvancedRigid2DTransform.h"
#include "itkAdvancedEuler3DTransform.h"
#include "itkAdvancedMatrixOffsetTransformBase.h"

namespace itk
{

/**
 * \class EulerGroup
 * \brief This class only contains an alias template.
 *
 */

template <unsigned int Dimension>
class EulerGroup
{
public:
  template <class TScalarType>
  using TransformAlias = AdvancedMatrixOffsetTransformBase<TScalarType, Dimension, Dimension>;
};

/**
 * \class EulerGroup<2>
 * \brief This class only contains an alias template for the 2D case.
 *
 */

template <>
class EulerGroup<2>
{
public:
  template <class TScalarType>
  using TransformAlias = AdvancedRigid2DTransform<TScalarType>;
};

/**
 * \class EulerGroup<3>
 * \brief This class only contains an alias template for the 3D case.
 *
 */

template <>
class EulerGroup<3>
{
public:
  template <class TScalarType>
  using TransformAlias = AdvancedEuler3DTransform<TScalarType>;
};


/**
 * This alias templates the EulerGroup over its dimension.
 */
template <class TScalarType, unsigned int Dimension>
using EulerGroupTemplate = typename EulerGroup<Dimension>::template TransformAlias<TScalarType>;


/**
 * \class EulerTransform
 * \brief This class combines the Euler2DTransform with the Euler3DTransform.
 *
 * This transform is a rigid body transformation.
 *
 * \ingroup Transforms
 */

template <class TScalarType, unsigned int Dimension>
class ITK_TEMPLATE_EXPORT EulerTransform : public EulerGroupTemplate<TScalarType, Dimension>
{
public:
  /** Standard ITK-stuff. */
  typedef EulerTransform                             Self;
  typedef EulerGroupTemplate<TScalarType, Dimension> Superclass;
  typedef SmartPointer<Self>                         Pointer;
  typedef SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EulerTransform, EulerGroupTemplate);

  /** Dimension of the domain space. */
  itkStaticConstMacro(SpaceDimension, unsigned int, Dimension);

  /** Typedefs inherited from the superclass. */

  /** These are both in Rigid2D and Euler3D. */
  typedef typename Superclass::ScalarType                ScalarType;
  typedef typename Superclass::ParametersType            ParametersType;
  typedef typename Superclass::NumberOfParametersType    NumberOfParametersType;
  typedef typename Superclass::JacobianType              JacobianType;
  typedef typename Superclass::OffsetType                OffsetType;
  typedef typename Superclass::InputPointType            InputPointType;
  typedef typename Superclass::OutputPointType           OutputPointType;
  typedef typename Superclass::InputVectorType           InputVectorType;
  typedef typename Superclass::OutputVectorType          OutputVectorType;
  typedef typename Superclass::InputCovariantVectorType  InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType OutputCovariantVectorType;
  typedef typename Superclass::InputVnlVectorType        InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType       OutputVnlVectorType;

  typedef typename Superclass::NonZeroJacobianIndicesType    NonZeroJacobianIndicesType;
  typedef typename Superclass::SpatialJacobianType           SpatialJacobianType;
  typedef typename Superclass::JacobianOfSpatialJacobianType JacobianOfSpatialJacobianType;
  typedef typename Superclass::SpatialHessianType            SpatialHessianType;
  typedef typename Superclass::JacobianOfSpatialHessianType  JacobianOfSpatialHessianType;
  typedef typename Superclass::InternalMatrixType            InternalMatrixType;

  /** Make sure SetComputeZYX() is available, also in 2D,
   * in which case, its just a dummy function.
   */
  void
  SetComputeZYX(const bool) // No override.
  {
    static_assert(SpaceDimension != 3, "This is not the specialization is 3D!");
  }


  /** Make sure GetComputeZYX() is available, also in 2D,
   * in which case, it just returns false.
   */
  bool
  GetComputeZYX(void) const // No override.
  {
    static_assert(SpaceDimension != 3, "This is not the specialization is 3D!");
    return false;
  }


protected:
  EulerTransform() = default;
  ~EulerTransform() override = default;

private:
  EulerTransform(const Self &) = delete;
  void
  operator=(const Self &) = delete;
};

template <class TScalarType>
class EulerTransform<TScalarType, 3> : public EulerGroupTemplate<TScalarType, 3>
{
public:
  /** Standard ITK-stuff. */
  typedef EulerTransform                     Self;
  typedef EulerGroupTemplate<TScalarType, 3> Superclass;
  typedef SmartPointer<Self>                 Pointer;
  typedef SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EulerTransform, EulerGroupTemplate);

  /** Dimension of the domain space. */
  itkStaticConstMacro(SpaceDimension, unsigned int, 3);


  /** Make sure SetComputeZYX() is available, also in 2D,
   * in which case, its just a dummy function.
   * \note This member function is only an `override` in 3D.
   */
  void
  SetComputeZYX(const bool arg) override
  {
    static_assert(SpaceDimension == 3, "This specialization is for 3D only!");

    typedef AdvancedEuler3DTransform<TScalarType> Euler3DTransformType;
    typename Euler3DTransformType::Pointer        transform = dynamic_cast<Euler3DTransformType *>(this);
    if (transform)
    {
      transform->Euler3DTransformType::SetComputeZYX(arg);
    }
  }


  /** Make sure GetComputeZYX() is available, also in 2D,
   * in which case, it just returns false.
   * \note This member function is only an `override` in 3D.
   */
  bool
  GetComputeZYX(void) const override
  {
    static_assert(SpaceDimension == 3, "This specialization is for 3D only!");

    typedef AdvancedEuler3DTransform<TScalarType> Euler3DTransformType;
    typename Euler3DTransformType::ConstPointer   transform = dynamic_cast<const Euler3DTransformType *>(this);

    if (transform)
    {
      return transform->Euler3DTransformType::GetComputeZYX();
    }
    return false;
  }


protected:
  EulerTransform() = default;
  ~EulerTransform() override = default;

private:
  EulerTransform(const Self &) = delete;
  void
  operator=(const Self &) = delete;
};

} // end namespace itk

#endif // end #ifndef itkEulerTransform_h
