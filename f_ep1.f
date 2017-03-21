      program p1

        real A(2,2)
        real b(2)

        integer p(2)

        logical sing

        b(1) = 6.0
        b(2) = 8.0

        A(1, 1) = 0.0
        A(1, 2) = 2.0
        A(2, 1) = 2.0
        A(2, 2) = 0.0

        call lurow(A, 2, p, sing)

        call ssrow(A, 2, p, b, sing)

        write (*,*) b

        stop
      end

      subroutine lurow (A, n, p, singular)
        integer n
        real A(n, n)
        integer p(n)
        logical singular

        integer i, j, k
        integer imax
        real temp

        singular = .FALSE.

        do 10 k = 1, n, 1
          imax = k
          do 20 i = k + 1, n, 1
            if (A(i, k)*A(i, k) .GT. A(imax, k)*A(imax,k)) imax = i
20        continue

c         se A(imax, k) = 0, A e' singular
          if (A(imax, k) .LT. 1.0E-20) then
            singular = .TRUE.
            return
          endif

          if (imax .NE. k) then
            do 30 j = 1, n, 1
              temp = A(imax, j)
              A(imax, j) = A(k, j) 
              A(k, j) = temp
30          continue
          endif

          p(k) = imax

          do 40 i = k + 1, n, 1
            A(i, k) = A(i, k) / A(k, k)
            do 50 j = k + 1, n, 1
              A(i, j) = A(j, j) - A(i, k)*A(k, j)
50          continue
40        continue
10      continue

        return
      end

      subroutine ssrow (A, n, p, b, singular)
        integer n
        real A(n, n)
        integer p(n)
        logical singular
        real b(n)

        integer i, j
        real temp

c       realizamos permutacoes
        do 60 i = 1, n, 1
          temp = b(i)
          b(i) = b(p(i))
          b(p(i)) = temp
60      continue
        do 70 i = 1, n, 1
          do 80 j = 1, i - 1, 1
            b(i) = b(i) - b(j) * A(i, j)
80        continue
70      continue

c       vamos resolver Ly = b e fazer b := y
        do 15 i = 1, n, 1
          do 25 j = 1, i - 2, 1
            b(i) = b(i) - A(i, j) * b(j)
25        continue
15      continue

c       vamos resolver Ax = y e fazer b := x
        do 90 i = n, 1, -1
          do 95 j = i + 1, n, 1
            b(i) = b(i) - A(i, j) * b(j)
95        continue

c         se A(i, i) = 0, A e' singular
          if (A(i, i) .LT. 1.0E-20) then
            singular = .TRUE.
            return
          endif
          b(i) = b(i) / A(i, i)
90      continue

        return
      end

